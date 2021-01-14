/*
File CriticalSection.hpp
Author: Chunhui Ma
Purpose: WRAPER FOR Critical_SECTION
*/

#pragma once
#include <Windows.h>


class CriticalSection {
	CRITICAL_SECTION m_cs;
public:
	inline CriticalSection() { InitializeCriticalSection( &m_cs ); }
	inline ~CriticalSection() { DeleteCriticalSection( &m_cs ); }

private:
	inline void Enter() { EnterCriticalSection( &m_cs ); }
	inline void Leave() { LeaveCriticalSection( &m_cs ); }

	friend class CSLock;
};

class CSLock {
	CriticalSection& m_csr;
public:
	inline CSLock( CriticalSection& cs ) : m_csr(cs) {
		m_csr.Enter();
	}
	inline ~CSLock() {
		m_csr.Leave();
	}
};
