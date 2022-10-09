
/* cNode.hpp
	
*/
   
#ifndef __CNODE_HPP__
#define __CNODE_HPP__

#include <string>
#include <vector>
#include <mutex>
//#include <cNodeA.hpp>

class cNode//: public cNodeA
{

	public:

		cNode( size_t _id, size_t _n, size_t _InitFood )
		:id(_id),x2(_id%_n),x1((_id-_id%_n)/_n),idCel(0),status(true),n(_n),usada(false), szNodeFood(_InitFood), tipoCel(0)
		//:cNodeA( _id, _n, _InitFood )
		{}
		
		~cNode();

		bool getIn( size_t _idCel, size_t _tp );

		bool getOut(void);

		bool getStatus(void);

		bool getUso(void);

		size_t getId(void);

		size_t getX1(void);

		size_t getX2(void);

		size_t showFood(void);

		size_t getFood(void);

		size_t getFood(size_t _food);

		void  putFood(size_t _food); /* inclui comido no  cNode */

		cNode* getMove( int _idx );
		
		void buildNet(std::vector<cNode*>& _vt );
		
		void setTipo( size_t _tp );
		
		size_t getTipo(void);

		int calcId( int _x1, int _x2);
		
		/* - Tratamento do vector de Poisons */
		/* inclui o veneno no Node */
		bool setPoison( std::string _poison );
		/* verifica se existe o veneno */
		bool verifPoison( std::string _poison );
		/* verifica se existe pelo menos um veneno */
		bool getExistPoison(void);
		
	private:
	
		int calcNovaPos(int _x, int _dx );
		
		size_t szNodeFood;
		size_t id;
		size_t n;
		int x1;
		int x2;
		bool status; /* true livre - false ocupado */
		bool usada;
		size_t idCel;
		size_t tipoCel;
		std::mutex mtx;
		
		/* conexão do Node no Grid - nodes vizinhos */
		cNode* ptrPos1 = nullptr;
		cNode* ptrPos2 = nullptr;
		cNode* ptrPos3 = nullptr;
		cNode* ptrPos4 = nullptr;
		cNode* ptrPos5 = nullptr;
		cNode* ptrPos6 = nullptr;
		cNode* ptrPos7 = nullptr;
		cNode* ptrPos8 = nullptr;
		
		/* vetor onde estão os venenos incluídos na cNode */
		std::vector<std::string> vPoison;
		
};


#endif /* __CNODE_HPP__ */