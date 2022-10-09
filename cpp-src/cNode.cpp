#include <cNode.hpp>
#include <algorithm>

/*
cNode::cNode( size_t _id, size_t _n, size_t _InitFood )
:id(_id),x2(_id%_n),x1((_id-_id%_n)/_n),idCel(0),status(true),n(_n),usada(false), szNodeFood(_InitFood), tipoCel(0)
{
	
}
*/
cNode::~cNode()
{
	
}

bool cNode::getIn( size_t _idCel, size_t _tp )
{
	//if( usada == true )// para nao retornar a um node já utilizado
	//	return false;
	if( status == false )
		return false;

	if( mtx.try_lock() )
	{
		idCel = _idCel;
		tipoCel = _tp;
		status = false;
		usada = true; /* indica que node já foi usado */
		mtx.unlock();
		return true;
	}else{
		return false;
	}
}

bool cNode::getOut(void)
{
	idCel = 0;
	tipoCel = 0;
	status = true; /*liberando node para ser ocupado */
	return true;
}

bool cNode::getStatus(void)
{
	return status;
}

bool cNode::getUso(void)
{
	return usada;
}

size_t cNode::getId(void)
{
	return id;
}

size_t cNode::getX1(void)
{
	return x1;
}

size_t cNode::getX2(void)
{
	return x2;
}

size_t cNode::showFood(void)
{
	return szNodeFood;
}

size_t cNode::getFood(void)
{
	if( !szNodeFood )
		return 0;
	
	if( szNodeFood == 1 )
	{
		szNodeFood--;
		return 1;
	}

	if( szNodeFood == 2 )
	{
		szNodeFood -= 2;
		return 2;
	}
	
	szNodeFood -= 3;
	
	return 3;
}

size_t cNode::getFood(size_t _food)
{
	size_t szRetFood = 0;
	if( !szNodeFood )
		return szRetFood;

	if( szNodeFood < _food )
	{
		szRetFood = szNodeFood;
		szNodeFood = 0;
	}else{
		szRetFood = _food;
		szNodeFood = szNodeFood - _food;
	}
	return szRetFood;
}

/* inclui comida no  cNode */
void cNode::putFood(size_t _food)
{
	if( _food > 0 )
		szNodeFood += _food;

}

cNode* cNode::getMove( int _idx )
{
	switch(_idx)
	{
		case 0:
			return this;
		case 1:
			return ptrPos1;
		case 2:
			return ptrPos2;
		case 3:
			return ptrPos3;
		case 4:
			return ptrPos4;
		case 5:
			return ptrPos5;
		case 6:
			return ptrPos6;
		case 7:
			return ptrPos7;
		case 8:
			return ptrPos8;
		default:
			return nullptr;
	};
	
}


void cNode::buildNet(std::vector<cNode*>& _vt )
{
	
	ptrPos1 = _vt[ (calcNovaPos(x1, 0)%n)*n + calcNovaPos(x2,-1)%n ];
	ptrPos2 = _vt[ (calcNovaPos(x1, 1)%n)*n + calcNovaPos(x2,-1)%n ];
	ptrPos3 = _vt[ (calcNovaPos(x1, 1)%n)*n + calcNovaPos(x2, 0)%n ];
	ptrPos4 = _vt[ (calcNovaPos(x1, 1)%n)*n + calcNovaPos(x2, 1)%n ];
	ptrPos5 = _vt[ (calcNovaPos(x1, 0)%n)*n + calcNovaPos(x2, 1)%n ];
	ptrPos6 = _vt[ (calcNovaPos(x1,-1)%n)*n + calcNovaPos(x2, 1)%n ];
	ptrPos7 = _vt[ (calcNovaPos(x1,-1)%n)*n + calcNovaPos(x2, 0)%n ];
	ptrPos8 = _vt[ (calcNovaPos(x1,-1)%n)*n + calcNovaPos(x2,-1)%n ];
	
}


void cNode::setTipo( size_t _tp )
{
	tipoCel = _tp;
}

size_t cNode::getTipo(void)
{
	return tipoCel;
}

int cNode::calcId( int _x1, int _x2)
{
	if( _x1 >= n || _x2 >= n )
		return -1;
	return _x1*n + _x2;
}
		
int cNode::calcNovaPos(int _x, int _dx )
{
	int x_1 = _x + _dx;
	
	if( x_1 < 0 )
	{
		x_1 = n + x_1;
		
	}else if( x_1 >= n ){
		x_1 = x_1%n;
	}
	
	return x_1;
}

bool cNode::setPoison( std::string _poison )
{
	if( !verifPoison( _poison ) )
	{
		vPoison.push_back(_poison);
		return true;
	}
	else
		return false;
}

bool cNode::verifPoison( std::string _poison )
{
	if( vPoison.end() != std::find(vPoison.begin(),vPoison.end(), _poison) )
		return true;
	else
		return false;

}

bool cNode::getExistPoison(void)
{
	if( vPoison.size() > 0 )
		return true;
	else
		return false;
}