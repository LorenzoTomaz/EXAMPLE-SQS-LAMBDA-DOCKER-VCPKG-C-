/* cNucleo.hpp

*/
#ifndef __CNUCLEO_HPP__
#define __CNUCLEO_HPP__

#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include <chrono>
#include <random>

#include <cNode.hpp>
#include <cLife.hpp>

using namespace std;

typedef int (*f_input) ( cLife*, double&);
typedef int (*f_output)( f_input, cLife*, double&, const std::string& );
using funcIn  = f_input;
using funcOut = f_output;

std::map<std::string, f_input>  mapIn; //{{"RND_INPUT",cNucleo::randRealTensor}};
										//"RND_INT", cNucleo::randIntTensor
										//"FOOD_INPUT",cNucleo::foodInput
										//"CHEMICAL_AGENT_INPUT",cNucleo::chemicalInput
std::map<std::string, funcOut>  mapOut; //{{"MOVE_AXIS",cNucleo::getPos}};
										//"FOOD_OUTPUT",cNucleo::foodOutput
										//"CHEMICAL_AGENT_REACTION",cNucleo::chemicalOutput

class cNucleo
{

	public:

		cNucleo()=delete;

		~cNucleo()=delete;

		static void fAction( cLife* _life ) 
		{
			funcIn fIn = nullptr;
			funcOut fOut = nullptr;
			for( auto vG : _life->vGenenoma )
			{
				if( !_life->getStatus() )
					return;

				fIn  = mapIn.find( vG.input_gene )->second;
				fOut = mapOut.find( vG.output_gene )->second;

				if( fOut && fIn )
				{
					fCall( fIn, fOut, _life, vG.param, vG.output_gene );
				}
				else
				{

					std::cout << "\n COMBINACAO NAO EXISTENTE --------------" << std::endl;

					std::cout << " INPUT_GENE  :[" << vG.input_gene << "]";
					if( !fIn )
						std::cout << "[NAO EXISTE]" << std::endl;
					else
						std::cout << "[EXISTE    ]" << std::endl;

					std::cout << " OUTPUT_GENE :[" << vG.output_gene << "]";
					if( !fOut )
						std::cout << "[NAO EXISTE]" << std::endl;
					else
						std::cout << "[EXISTE    ]" << std::endl;

					std::cout << " PARAMETRO   :[" << vG.param << "]" << std::endl;
					exit(1);
				}

				fIn = nullptr;
				fOut = nullptr;
			}

		}

		/* CallBack - processa as combinações */
		static void fCall( funcIn _fIn, funcOut _fOut, cLife* _life, double& _in, const std::string& _strGeneOut )
		{
			_fOut( _fIn, _life, _in, _strGeneOut );
		}

		/* INPUT´s -----------------------------------------------*/
		/* INPUT RND_INPUT */
		static int randRealTensor( cLife* _life, double& _in )
		{
			double input     = randReal( -1.0, 1.0 );
			double moveVal   = tanh( input );
			double probVal   = ( randReal(0,1) < abs(moveVal) ? 1 : 0 );
			double signumVal = ( moveVal < 0.00 ? -1.00 : 1.00 );
			return (int)( signumVal * probVal );
		}

		/* INPUT RND_INT */
		static int randIntTensor( cLife* _life, double& _in )
		{
			double input     = randInt( -1.0, 1.0 );
			double moveVal   = tanh( input );
			double probVal   = ( randInt(0,1) < abs(moveVal) ? 1 : 0 );
			double signumVal = ( moveVal < 0.00 ? -1.00 : 1.00 );
			return (int)( signumVal * probVal );
		}

		/* INPUT FOOD_INPUT */
		static int foodInput( cLife* _life, double& _in )
		{
			return (int)(_in);
		}

		/* CHEMICAL_AGENT_INPUT */
		static int chemicalInput( cLife* _life, double& _in )
		{

			// _param : representa índice da probabilidade da cLife não sobreviver ao 
			//          contato com o veneno.
			// Retorno da função que sorteia o efeito do veneno
			// true : vive
			// false: não vive
			unsigned root = std::chrono::system_clock::now().time_since_epoch().count();
			std::mt19937 seed(root);
			std::binomial_distribution<int> dist(1,_in);

			if( !dist(seed) )
				return 1;  /* resistiu */
			else
				return 0;  /* não resistiu */

		}
		/* OUTPUT´s -------------------------------------------------------  */
		/* OUTPUT MOVE_AXIS */
		static int getPos( f_input _funcIn, cLife* _life, double& _in ,  const std::string& _strGeneOut )
		{
			int newX = 0;
			int newY = 0;
			int iMov = 0;
			int iCtrl = 0;
			cNode* tmp = nullptr;

			do{

				newX = _funcIn( _life, _in );
				newY = _funcIn( _life, _in );

				if( newX ==  0 && newY ==  0 )
						iMov = 0;
				else if( newX ==  0 && newY == -1 )
						iMov = 1;
				else if( newX ==  1 && newY == -1 )
						iMov = 2;
				else if( newX ==  1 && newY ==  0 )
						iMov = 3;
				else if( newX ==  1 && newY ==  1 )
						iMov = 4;
				else if( newX ==  0 && newY ==  1 )
						iMov = 5;
				else if( newX == -1 && newY ==  1 )
						iMov = 6;
				else if( newX == -1 && newY ==  0 )
						iMov = 7;
				else if( newX == -1 && newY == -1 )
						iMov = 8;

				tmp = _life->getNode()->getMove(iMov);
				if( tmp == _life->getNode() )
					break;
				if( !tmp->getIn(_life->getId(), _life->getTipo()) )
				{
					tmp = nullptr;
					iCtrl++;
				}else{
					_life->getNode()->getOut();
					_life->setNode(tmp);
					iCtrl = 0;
				}

				if( iCtrl == 10 )
					break;

			}while(!tmp);

			return 0;
		}

		/* OUTPUT FOOD_OUTPUT */
		static int foodOutput( f_input _funcIn, cLife* _life, double& _in,  const std::string& _strGeneOut )
		{
				int qtd_food = _funcIn( _life, _in );
			_life->putEnergia(_life->getNode()->getFood((size_t)qtd_food));

			/* deve haver um gene para metabolismo */
			_life->consumeEnergia(1);
			//_life->getNode()->putFood(1); /* retorna para o meio energia */

			if( _life->getEnergia() <= 0 )
			{
				_life->setStatusFalse();
				_life->getNode()->getOut();
			}
			return 0;
		}

		/* OUTPUT CHEMICAL_AGENT_REACTION */
		static int chemicalOutput( f_input _funcIn, cLife* _life, double& _in,  const std::string& _strGeneOut )
		{
			if( !_life->getNode()->verifPoison( _strGeneOut ) )
				return 0;

			if( _in >= 1.00 || _in <= 0.01 )
				return 0;

			/* regra de input */
			int iRet = _funcIn( _life, _in );

			if( 1 == iRet )
				_in *= _in/2.00;
			else
			{
				/* retorna a energia restante na cLife para cNode */
				_life->getNode()->putFood(_life->getEnergia());
				_life->setStatusFalse();
				_life->getNode()->getOut();
			}
			return 0;
		}

	private:

		static double randReal( double _low, double _high )
		{
			unsigned num = std::chrono::system_clock::now().time_since_epoch().count();
			std::mt19937 seed(num);
			std::uniform_real_distribution<double> dist(_low, _high);
			return dist(seed);
		}

		static double randInt( double _low, double _high )
		{
			std::random_device seed;
			std::uniform_int_distribution<int> dist( _low, _high );
			return dist(seed);
		}

};



#endif /*__CNUCLEO_HPP__*/
