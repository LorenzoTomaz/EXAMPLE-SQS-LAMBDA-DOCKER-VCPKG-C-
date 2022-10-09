#include <cLife.hpp>
#include <iostream>


using namespace std;


void cLife::getFirst(std::vector<cNode*>& _vNodes, size_t _idNode )
{
	if( ptrNode )
	{
		cout << "Id [" << id << "] Tipo [" << tipo << "] Ja esta alocado." << endl;
		return;
	}
	
	if( _vNodes[_idNode]->getIn(id, tipo) ){
		ptrNode = _vNodes[_idNode];
	}else
		cout << "[" << id << "] Nao Entrou na celula [" << _idNode << "]\n";
	
}

void cLife::getFirst(std::vector<cNode*>& _vNodes, int _x1, int _x2 )
{
	
	if( ptrNode )
	{
		cout << "Id [" << id << "] Tipo [" << tipo << "] Ja esta alocado." << endl;
		return;
	}
	
	/* Utiliza o primeiro registro de -vNodes apanas para calcular */
	size_t idNode = _vNodes[0]->calcId(_x1,_x2);
	
	//cout << " ID [" << idNode << "](" << _x1 << ", " <<  _x2 << ")\n";
	if( _vNodes[idNode]->getIn(id, tipo) ){
		ptrNode = _vNodes[idNode];
	}else
		cout << "[" << id << "] Nao Entrou na celula [" << idNode << "]\n";
	
}


bool cLife::getStatusLife(void)
{
	return stsLife;
}


size_t cLife::getTipo(void)
{
	return tipo;
}

size_t cLife::getId(void)
{
	return id;
}


bool cLife::movePos(void)
{
	if( !stsLife )		
		return false;
	
	cNode* tmp = nullptr;
	size_t tmp_id = 0;
	int iCtrl = 0;
	do{
		//tmp = ptrNode->getMove(getRand()); // substituindo por cNucleo 
		if( !tmp->getIn(id, tipo) )
		{
			tmp = nullptr;
			iCtrl++;
			
		}else{
			szEnergia += tmp->getFood();
			//ptrNode->getOut();
			ptrNode = tmp;
			iCtrl = 0;
			
			
			
		}
		if( iCtrl == 10 )
			break;
		
	}while(!tmp);
	
	szEnergia -= 1;
	
	if( !szEnergia )
	{
		tmp->getOut();
		stsLife = false; 
	}
	return stsLife;
}


bool cLife::actionLife(void)
{
	if( !stsLife )
		return false;

	cNode* tmp = nullptr;
	//size_t tmp_id = 0;
	int iCtrl = 0;

	return stsLife;
}


cNode* cLife::getNode(void){
	return ptrNode;
}

int cLife::getRand(void)
{
	std::random_device rdLife;
	std::mt19937 gerLife(rdLife());

	std::uniform_int_distribution<> dist(0, 8);
	return dist(gerLife);
}

bool cLife::randPoison( double _param )
{
	// _param : representa índice da probabilidade da cLife não sobreviver ao 
	//          contato com o veneno.
	// Retorno da função que sorteia o efeito do veneno
	// true : vive
	// false: não vive

	unsigned root = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 seed(root);
	std::binomial_distribution<int> dist(1,_param);

	if( !dist(seed) )
		return true;
	else
		return false;

}

void cLife::setGenoma(std::vector<TPGENE>& _vGene, size_t _tipo  )
{
	if( _tipo == tipo )
	{
		for( auto a : _vGene )
			vGenenoma.push_back(a);
	}
}


double cLife::randReal( double _low, double _high )
{
	unsigned root = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 seed(root);
	std::uniform_real_distribution<double> dist(_low, _high);
	return dist(seed);
}

int cLife::randTensor(void)
{
	double inputVal  = randReal(-1.0, 1.0);
	double moveVal   = tanh( randReal( -1.0, 1.0 ) );
	double probVal   = ( randReal(0,1) < abs(moveVal) ? 1 : 0 );
	double signumVal = ( moveVal < 0.00 ? -1.00 : 1.00 );
	return (int)(signumVal * probVal);
}


int cLife::getRandPos(void)
{
	int newX = randTensor();
	int newY = randTensor();
	int iMov = 0;

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

	return iMov;

}


void cLife::setNode(cNode* _ptr ){
	ptrNode = _ptr;
}
std::vector<TPGENE>& cLife::getGenoma(void)
{
	return vGenenoma;
}

size_t cLife::getEnergia(void)
{
	return szEnergia;
}

void cLife::consumeEnergia( size_t _n )
{
	if( _n > szEnergia )
	{
		szEnergia = 0;
		return;
	}
	szEnergia -= _n;
}

void cLife::putEnergia( size_t _n )
{
	szEnergia += _n;
}

void cLife::setStatusFalse(void)
{
	stsLife = false;
}

void cLife::setStatusTrue(void)
{
	stsLife = true;
}

bool cLife::getStatus(void)
{
	return stsLife;
}

