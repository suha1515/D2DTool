#pragma once

#ifndef __FUNCTOR_H__
template<typename T,typename T2>
class CLess
{
public:
	bool operator()(T pObjectA, T pObjectB)
	{
		return pObjectA->GetComponent<T2>()->GetPosition().y < pObjectB->GetComponent<T2>()->GetPosition().y;
	}
};


#define __FUNCTOR_H__
#endif