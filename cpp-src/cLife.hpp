/* cLife.hpp

*/
#ifndef __CLIFE_HPP__
#define __CLIFE_HPP__

#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <algorithm>
#include <functional>
#include <thread>
#include <mutex>
#include <random>
#include <chrono>
#include <typeinfo>
#include <cstdlib>
#include <string>

#include "cNode.hpp"

/* estrutura genoma */
typedef struct _gene{
	std::string input_gene;
	std::string output_gene;
	double param;
}TPGENE;

class cLife
{
	public:
		cLife(size_t _id, size_t _tp, size_t _energiaInicial)
		:id(_id),tipo(_tp),stsLife(true), szEnergia(_energiaInicial){}

		void getFirst(std::vector<cNode*>& _vNodes, size_t _idNode );

		void getFirst(std::vector<cNode*>& _vNodes, int _x1, int _x2 );

		bool getStatusLife(void);

		size_t getTipo(void);

		size_t getId(void);

		bool movePos(void);

		bool actionLife(void);

		void setGenoma(std::vector<TPGENE>& _vGene, size_t _tipo );

		cNode* getNode(void);

		void setNode(cNode* _ptr );

		std::vector<TPGENE>& getGenoma(void);

		size_t getEnergia(void);

		void consumeEnergia( size_t _n );

		void putEnergia( size_t _n );

		void setStatusFalse(void);

		void setStatusTrue(void);

		bool getStatus(void);

		/* vetor que contém o genoma do ser vivo */
		std::vector<TPGENE> vGenenoma;

	private:

		int getRand(void);
		int getRandPos(void);
		bool randPoison( double _param );
		double randReal( double _low, double _high );
		int randTensor(void);

		size_t id; /* deve ser maior que zero */
		size_t tipo;
		cNode* ptrNode = nullptr;
		bool stsLife; /* true esta vivo - false está morto */
		size_t szEnergia; /* quantidade de energia vital */

};


#endif /* __CLIFE_HPP__ */