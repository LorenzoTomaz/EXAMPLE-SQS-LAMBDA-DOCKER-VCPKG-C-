/* cBoard.hpp 
 * 
 * 
 *
 * 
 */

#ifndef __CBOARD_HPP__
#define __CBOARD_HPP__

#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <algorithm>
#include <functional>
#include <thread>
#include <mutex>
#include <chrono>
#include <typeinfo>
#include <cstdlib>
#include <random>

#include "cNode.hpp"
#include "cLife.hpp"
#include "cNucleo.hpp"

using namespace std;

std::random_device rd;
std::mt19937 ger(rd());


/* estrutura de resumo para verificação */
/* usado comente para o desenvolvimento e
   mostrar em tela da forma mais simples */
typedef struct strResumo{
	size_t sequencia;
	size_t qtde_1;
	size_t qtde_2;
	size_t qtde_intocados;
	size_t reserva;
}TPRESUMO;


class cBoard
{

	public:

		cBoard()=delete;
		/**
		 * @brief Construct a new cBoard object
		 * 
		 * @param _LateralSize = quantidade de cnodes da lateral do quadrado que será contruido para Board.
		 * @param _InitNodeFood = Quantidade inicial de comida/energia para cada node da board.
		 * @param _initLifeFood = Quantidade inicial de comida/energia para cada life.
		 * @param _qtdeLife = deve ser um par que será divido por dois, para definir a quantidade de life iniciais
		 * 					  para cada tipo (1 e 2) 
		 */
		cBoard(size_t _LateralSize, size_t _InitNodeFood, size_t _initLifeFood, size_t _qtdeLife )
		:n(_LateralSize),szInitNodeFood(_InitNodeFood),szLifeEnergiaInicial(_initLifeFood),
		szTotalLifes(_qtdeLife)
		{
			if( 0 != szTotalLifes%2 )
			{
				std::cout << "ERRO: Deve ser um numero par!!!" << std::endl;
				exit(1);
			}
			/*configuração do tratamente que cada gene deve receber
				relacionando o gene a função processadora */
			/* mapa de Genes e Funções Input */
			mapIn.insert({"RND_INPUT",cNucleo::randRealTensor});
			mapIn.insert({"RND_INT",cNucleo::randIntTensor});
			mapIn.insert({"FOOD_INPUT",cNucleo::foodInput});
			mapIn.insert({"CHEMICAL_AGENT_INPUT",cNucleo::chemicalInput});
			/* mapa de Genes e Funçoes Output */
			mapOut.insert({"MOVE_AXIS",cNucleo::getPos});
			mapOut.insert({"FOOD_OUTPUT",cNucleo::foodOutput});
			mapOut.insert({"CHEMICAL_AGENT_REACTION",cNucleo::chemicalOutput});

			/* inicializa threads */
			for( size_t sz = 0; sz < nr_threads; ++sz )
			{
				vThread.push_back(thread(workMove, std::ref(wQueue) , std::ref(mtx_1)));
				vThread.push_back(thread(workMove, std::ref(wQueue1), std::ref(mtx_2)));
			}

		}

		virtual ~cBoard(){}

		int start( void );

		static void workMove(queue<cLife*>& _qLife, std::mutex& mtx_thr )
		{
			cLife* ptr = nullptr;

			do{
				mtx_thr.lock();
					if(_qLife.size() > 0 )
					{
						ptr = _qLife.front();
						if( ptr )
							_qLife.pop();
					}
				mtx_thr.unlock();

				if( ptr ){
					cNucleo::fAction( ptr );
					ptr = nullptr;
				}else{
					std::this_thread::sleep_for (std::chrono::microseconds(100));
				}
			}while(stsThreadWork);

		}

		void setGeneTipo1( std::string _geneIn, std::string _geneOut, double _param );

		void setGeneTipo2( std::string _geneIn, std::string _geneOut, double _param );

	private:

		/* função para gerar números randômicos */
		int random( int _low, int _high );

		/* listagem da Grade de Nodes */
		bool lista( vector<cNode*>& _vt, size_t _n, TPRESUMO& _resumo );

		/*Função que reordena randomicamente as listas de cLifes e produz
		novas filas de processamento */
		bool embaralha( std::list<cLife*>& _lstLife, std::queue<cLife*>& _qLife, size_t  _tipo );

		/* função que gera a posição inicial para cada cLife */
		bool geraRandomPos( std::vector<size_t>& _vGera, size_t _qtde, size_t _limite );

		/* função que inclui randomicamente um veneno na grid  */
		bool putRandomPoison( vector<cNode*>& _vNodes, size_t _qtde, std::string _poison );


	/* parametros iniciais */
	const size_t n;// 30 
	const size_t szInitNodeFood;//10
	const size_t szLifeEnergiaInicial; //1
	size_t szTotalLifes;// = 100;  // nr. bacterias a serem criadas
								// metade em cada tipo.
								// Portanto deverá ser um número par.

	/* vetor, list e queues de comunicação para montagem do ecossistema */
	vector<cNode*> vNodes;
	list<cLife*> lstLife;
	queue<cLife*> wQueue;  /* file de trabalho para threads */
	queue<cLife*> wQueue1; /* file de trabalho para threads */
	/* cria e inicia threads para tratar o movimento */
	std::mutex mtx_1;
	std::mutex mtx_2;
	const size_t nr_threads = 2; /*  Define a quantidade utilizada em cada pool de processamento por tipo de "ser vivo". Nr total de threads será o dobro do apontado nesta variável. */
	/* vetor controle para processamento de threads */
	vector<thread> vThread;
	/* controle de threads */
	static bool stsThreadWork;


		/* cria Genoma */
	std::vector<TPGENE> vGeneBase1; //{"RND_INPUT","MOVE_AXIS",0.00},
									//  {"CHEMICAL_AGENT_INPUT","CHEMICAL_AGENT_REACTION",0.60},
									//  {"FOOD_INPUT","FOOD_OUTPUT", 3.00}
									//};

	std::vector<TPGENE> vGeneBase2;// = {{"RND_INT","MOVE_AXIS",0.00},
								  //	  {"CHEMICAL_AGENT_INPUT","CHEMICAL_AGENT_REACTION",0.60},
								  //	  {"FOOD_INPUT","FOOD_OUTPUT", 3.00}
								  //	};
};

bool cBoard::stsThreadWork = true;



void cBoard::setGeneTipo1( std::string _geneIn, std::string _geneOut, double _param )
{
	TPGENE gene;
	gene.input_gene = _geneIn;
	gene.output_gene = _geneOut;
	gene.param=_param;
	vGeneBase1.push_back(gene);
}

void cBoard::setGeneTipo2( std::string _geneIn, std::string _geneOut, double _param )
{
	TPGENE gene;
	gene.input_gene = _geneIn;
	gene.output_gene = _geneOut;
	gene.param=_param;
	vGeneBase2.push_back(gene);
}

/* função para gerar números randômicos */
int cBoard::random( int _low, int _high )
{
	std::uniform_int_distribution<> dist(_low, _high);
	return dist(ger);
}

/* listagem da Grade de Nodes */
bool cBoard::lista( vector<cNode*>& _vt, size_t _n, TPRESUMO& _resumo )
{
	int iTotLivre = 0;
	int iLifes = 0;
	int iQtdTp1 = 0;
	int iQtdTp2 = 0;
	int iQtdFood = 0;
	for( auto b : _vt )
	{
		iQtdFood += b->showFood();
		if( !b->getStatus() )
		{
			iLifes++;
			if( b->getTipo() == 1 )
				iQtdTp1++;
			else if( b->getTipo() == 2 )
				iQtdTp2++;
		}
		if( b->getStatus() && !b->getUso() ) 
			iTotLivre++;
	}
	
	cout << " Lifes     : [" << iLifes << "]\n";
	cout << " Tipo 1    : [" << iQtdTp1 << "]\n";
	cout << " Tipo 2    : [" << iQtdTp2 << "]\n";
	cout << " Intocados : [" << iTotLivre << "]\n";
	cout << " Reserva   : [" << iQtdFood << "]\n";
	for( auto a : _vt )
	{
		//cout << "(" << a->getX1() << ", " << a->getX2() << ") Id :" << a->getId() << " ";
		if( !a->getStatus() )
		{
			if( a->getTipo() == 1 ){
				cout << " X ";
			}else if( a->getTipo() == 2 ){
				cout << " @ ";
			}else 
				cout << " T ";
			
		}else if( a->showFood() ){

			if( a->getExistPoison() )
				cout << "   ";// V
			else if( a->showFood() > 9 )
				cout << "   ";//*
			else
				cout << " " << " " << " ";//a->showFood()

		}else
		if( a->getExistPoison() )
				cout << "   ";//V
		else
			cout << "   ";
		
		if( a->getId() > 0 && (a->getId()+1)%_n == 0 )
			cout << endl;
	}
	
	if( iQtdTp1 > 0 && iQtdTp2 > 0 )
		return true;
	else
	{
		_resumo.qtde_1 = iQtdTp1; 
		_resumo.qtde_2 = iQtdTp2; 
		_resumo.qtde_intocados = iTotLivre;
		_resumo.reserva = iQtdFood;
		return false;
	}
}

/*Função que reordena randomicamente as listas de cLifes e produz
  novas filas de processamento */
bool cBoard::embaralha( std::list<cLife*>& _lstLife, std::queue<cLife*>& _qLife, size_t  _tipo )
{
	if( !_lstLife.size() )
		return false;
	
	std::vector<cLife*> vLife;
	
	for_each( _lstLife.begin(), _lstLife.end(), [&vLife,&_tipo] (auto _b){
		if( _b->getTipo() == _tipo)
			vLife.push_back(_b);
	});
	
	if( !vLife.size() )
		return false; 
	
	std::shuffle(vLife.begin(),vLife.end(), ger);
	
	for( auto a : vLife )
	{
		_qLife.push(a);
	}
	
	if( !_qLife.size() )
		return false;
	
	return true;
}

/* função que gera a posição inicial para cada cLife */
bool cBoard::geraRandomPos( std::vector<size_t>& _vGera, size_t _qtde, size_t _limite )
{
	size_t nr = 0;
	std::vector<size_t>::iterator it;
	do{
		nr = random(0,_limite);
		if( _vGera.end() == find(_vGera.begin(), _vGera.end(),nr) )
		{
			_vGera.push_back(nr);
		}
	}while(_vGera.size() < _qtde );

	return true;
}

/* função que inclui randomicamente um veneno na grid  */
bool cBoard::putRandomPoison( vector<cNode*>& _vNodes, size_t _qtde, std::string _poison )
{
	size_t nr = 0;
	size_t szContador = 0;
	const size_t limite = _vNodes.size() - 1;
	do{
		nr = random(0,limite);
		if( _vNodes[nr]->setPoison(_poison) )
		{
			szContador++;
		}
	}while( szContador < _qtde );

	return true;
}


int cBoard::start( void  )
{

	vNodes.clear();
	lstLife.clear();
	while(!wQueue.empty())
	{
		wQueue.pop();
	}
	while(!wQueue1.empty())
	{
		wQueue1.pop();
	}

	/* tamanho : numero de cNode no Board */
	const size_t tam = n*n;
	/* preparação do espaço para montagem do ecossistema */
	/* cria nodes */
	for( size_t i=0; i < tam; ++i )
		vNodes.push_back(new cNode(i, n, szInitNodeFood));
	/* interliga parte e cria a net nodes */
	for( auto a : vNodes )
		a->buildNet(vNodes);

	/* inclui poison na net nodes */
	putRandomPoison(vNodes, 30, "CHEMICAL_AGENT_REACTION" ); // esse ser usado

	/* inclusão de "seres vivos" */
	size_t szSeq = 0; /* iD de cada bactéria */
	vector<size_t> vIniPos;
	geraRandomPos(vIniPos,szTotalLifes,(tam-1));
	for( int i=0; i < szTotalLifes; ++i)
	{
		if( 0 == i%2 )
			lstLife.push_back(new cLife(++szSeq,1,szLifeEnergiaInicial));
		else
			lstLife.push_back(new cLife(++szSeq,2,szLifeEnergiaInicial));
	}

	/* Verifica existência do Genoma*/
	if( vGeneBase1.size() == 0 || vGeneBase2.size() == 0 )
	{
		std::cout << " Genoma Vazio - Nao pode funcionar assim!!!" << std::endl;
		exit(1);
	}

	/* inclui genoma no seres vivos */
	for( auto a : lstLife )
	{
		a->setGenoma(vGeneBase1,1);
		a->setGenoma(vGeneBase2,2);
	}

	/* inclusão de seres "vivos" no ecossistema (gride nodes) */
	int iList = 0;
	for( auto a : lstLife )
	{
		a->getFirst(vNodes, vIniPos[iList]);
		iList++;
	}

	/* inicia interações */
	int iCtrl = 0;
	TPRESUMO var_resumo;
	var_resumo.sequencia = iCtrl;
	var_resumo.qtde_1 = 0;
	var_resumo.qtde_2 = 0;
	var_resumo.qtde_intocados = 0;
	var_resumo.reserva = 0;	

	for(;; ++iCtrl )
	{
		/* retirar  lista o ecossistema - não vai fazer parte da solução - Inicio */
		system("clear");
		cout << "\n====================================\n";
		cout << " Sequencia       :[" << iCtrl << "]\n"; 
		cout << " List Lifes Size :[" << lstLife.size() << "]\n";
		if( !lista(vNodes,n, var_resumo) )
		{
			var_resumo.sequencia = iCtrl;
			break;
		}
		/* retirar lista o ecossistema - não vai fazer parte da solução - fim */
		
		/* movimentação*/
		mtx_1.lock();
		mtx_2.lock();
		
		/* produz as filas para processamento de cLifes
		   reordenando de forma aleatória a sequencia 
		   de "indivíduos a serem processados, 
		   para que essa ordem não seja sempre a mesma*/
		embaralha(lstLife, wQueue , 1);
		embaralha(lstLife, wQueue1, 2);
		
		mtx_1.unlock();		
		mtx_2.unlock();
		
		/* aguarda que threads tratem todos os registros de seres "vivos" */
		do{
			if(wQueue.size()> 0 || wQueue1.size() > 0 )
				std::this_thread::sleep_for (std::chrono::microseconds(100));
		}while(wQueue.size() || wQueue1.size() );
		
		/* retira cLife com status life = false */
		lstLife.remove_if([]( auto _node){ 
				bool sts = false;
				if( !_node->getStatusLife() )
				{
					delete _node;
					sts = true;
				}
				return sts; 
			});
			
		/* verifica a existencia de seres "vivos" no ecossistema, se não houverem encerra o processamento */
		if( lstLife.size() == 0 )
			break;
		else
			std::this_thread::sleep_for (std::chrono::microseconds(150000));/* apenas para dar o efeito "Irmãos Lumière" */
	}	
	
	/* Realiza a finalização das threads usadas no proceso. */
	stsThreadWork = false; 
	for( auto& th : vThread )
		th.join();

	/* Resumo */
	cout << "\nResumo========================\n";
	cout << "Nr. Interacoes    : [" << var_resumo.sequencia << "]\n";
	cout << "Quantidades Finais-----\n";
	cout << " Tipo 1           : [" << var_resumo.qtde_1 << "]\n";
	cout << " Tipo 2           : [" << var_resumo.qtde_2 << "]\n";
	cout << " Nodes s/ Acesso  : [" << var_resumo.qtde_intocados << "]\n";
	cout << " Reserva Energia  : [" << var_resumo.reserva << "]\n";
	
	cout << "\nPosicoes Iniciais : \n";
	for( auto a : vIniPos )
		cout << a << " ";
	cout << endl;
	

	if( var_resumo.qtde_1 > var_resumo.qtde_2 )
		return 1; // tipo 1 vence 
	else if( var_resumo.qtde_1 < var_resumo.qtde_2 )
		return 2; // tipo 2 vence 
	else 
		return 0; // empate

	return 0;
}


#endif /*__CBOARD_HPP__*/