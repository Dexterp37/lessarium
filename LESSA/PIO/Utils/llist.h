/** 
 *	\file llist.h 
 *	\class PIO::Utils::CSingleLinkedList llist.h "PIO/Utils/llist.h"
 *	\class PIO::Utils::CSingleLinkedList::_LLIST_NODE llist.h "PIO/Utils/llist.h"
 *	License: zlib/libpng
 *
 *	Crates and manipulates a singly linked list. Allows to define a custom DeleteFunc
 *  which is called to free memory associated with the T element
 *
 *	\author Alessio Pierluigi Placitelli
 *	\version 0.1
 *	\todo
 *		- Add "Iterate()" member to _LLIST_NODE and make m_pNext private. Iterate should return m_pNext;
 */
//$log$ 29/06/2006 Initial version

#ifndef _LLIST_H
#define _LLIST_H

namespace PIO
{
	namespace Utils
	{
		typedef LBool (*L_DELETEFUNC_PTR)(LVoid*);	///< L_DELETEFUNC_PTR prototype

		template<class T>
		class CSingleLinkedList
		{
		public:
			CSingleLinkedList() : m_pList(LNull), m_pTailItem(LNull), m_uNumElements(0), ItemDelFunc(LNull) {};
			~CSingleLinkedList();

			/**
				Returns the number of elements in the list

				\return Returns the number of elements in the list (the "length" of the list)
			*/	
			inline LUint GetNumElements() { return m_uNumElements; };

			/**
				Adds pElem to the head of our list

				\param[in] pElem A pointer of type T which points to the element to add
				\return Returns LTrue if element has been added, LFalse otherwise
				\attention The function doesn't check for pElem to be valid
			*/	
			LBool	AddHead(T * pElem);			

			/**
				Appends pElem to the tail of our list

				\param[in] pElem A pointer of type T which points to the element to add
				\return Returns LTrue if element has been added, LFalse otherwise
				\attention The function doesn't check for pElem to be valid
			*/		
			LBool	AddTail(T * pElem);			

			/**
				Removes element pElem from the list

				\param[in] pElem A pointer of type T which points to the element to remove
				\param[in] bFreeMemory If LTrue, element is deleted from memory
				\return Returns LTrue if element has been added, LFalse otherwise
				\attention The function doesn't check for pElem to be valid
			*/	
			LBool	Remove(T * pElem, LBool bFreeMemory = LTrue);

			/**
				Removes all elements from the list

				\param[in] bFreeMemory Tells the function to free memory associated with elements
			*/	
			LVoid	RemoveAll(LBool bFreeMemory = LTrue);

			/**
				Sets a custom delete function

				\param[in] theDelFunc A L_DELETEFUNC_PTR type function which is called when
				an object is being deleted

				\see L_DELETEFUNC_PTR
			*/	
			LVoid   SetDeleteFunc(L_DELETEFUNC_PTR theDelFunc) {ItemDelFunc = theDelFunc; }

		public:
			class _LLIST_NODE
			{
			public:
				_LLIST_NODE() : m_pElement(LNull), m_pNext(LNull) {};
				~_LLIST_NODE(){};

				T				*m_pElement;	///< Pointer to element
				_LLIST_NODE		*m_pNext;		///< Pointer to the next item in our list
			};

			/**
				Returns a pointer to the head of the list

				\return Returns a pointer to the first element of the list, LNull if empty
				\see _LLIST_NODE
			*/	
			inline _LLIST_NODE	*GetHead() const { return m_pList; };

			/**
				Returns a pointer to the tail of the list

				\return Returns a pointer to the last element of the list, LNull if empty
				\see _LLIST_NODE
			*/	
			inline _LLIST_NODE	*GetTail() const { return m_pTailItem; };

		private:
			LUint				m_uNumElements;	///< Number of elements in the list
			_LLIST_NODE			*m_pList;		///< Our list
			_LLIST_NODE			*m_pTailItem;	///< List tail item
			L_DELETEFUNC_PTR	ItemDelFunc;	///< Pointer to custom delete function

			/**
				Reset variables

				\note This gets called by destructor 
			*/	
			LVoid	Clear();

			/**
				Frees memory and empties the list

				\note This gets called by destructor
			*/	
			LVoid	Destroy();
		};

		template<class T> CSingleLinkedList<T>::~CSingleLinkedList()
		{
			Destroy();
			Clear();
		}

		template<class T> LVoid CSingleLinkedList<T>::Clear()
		{
			m_pList			= LNull;
			m_pTailItem		= LNull;
			m_uNumElements	= 0;
		}

		template<class T> LVoid CSingleLinkedList<T>::Destroy()
		{
			RemoveAll();
		}

		template<class T> LBool CSingleLinkedList<T>::AddHead(T * pElem)
		{
			// -> Create a structure to hold our element info
			_LLIST_NODE *pNode = new _LLIST_NODE;
			if(!pNode)
				return LFalse;

			pNode->m_pElement	= pElem;
			pNode->m_pNext		= m_pList;

			if(!m_pList)
				m_pTailItem = pNode;	// Initialize tail item

			// -> Link it to head!
			m_pList = pNode;

			++m_uNumElements;

			return LTrue;
		}

		template<class T> LBool CSingleLinkedList<T>::AddTail(T * pElem)
		{
			if(!m_pTailItem)	// && !m_pList ?
				return AddHead(pElem);

			// -> Create a structure to hold our element info
			_LLIST_NODE *pNode = new _LLIST_NODE;
			if(!pNode)
				return LFalse;

			pNode->m_pElement	= pElem;
			pNode->m_pNext		= LNull;

			m_pTailItem->m_pNext = pNode;
			m_pTailItem = pNode;

			++m_uNumElements;

			return LTrue;
		}

		template<class T> LBool CSingleLinkedList<T>::Remove(T * pElem, LBool bFreeMemory)
		{
			if(!m_pList)
				return LFalse;

			// -> If our element is on list head, remove it
			if(m_pList->m_pElement == pElem)
			{
				_LLIST_NODE *pBackup = m_pList;
				m_pList = m_pList->m_pNext;

				if(!bFreeMemory)
					pBackup->m_pNext = LNull;

				if(pBackup == m_pTailItem)
					m_pTailItem = LNull;

				if(!ItemDelFunc || !ItemDelFunc(pBackup->m_pElement))
				{
					delete pBackup->m_pElement;
					pBackup->m_pElement = LNull;
				}

				delete pBackup;

				--m_uNumElements;

				return LTrue;
			}

			_LLIST_NODE *pTemp = m_pList;

			while(pTemp->m_pNext)
			{
				// To simplify
				_LLIST_NODE *pToTest = pTemp->m_pNext;	
				  
				// We found our VIP
				if(pToTest->m_pElement == pElem)
				{
					_LLIST_NODE *pBackup = pToTest;
					pTemp->m_pNext = pToTest->m_pNext;

					if(!bFreeMemory)
						pBackup->m_pNext = LNull;

					if(pToTest = m_pTailItem)
					{
						m_pTailItem = pTemp;	// Update tail
					}

					// Allow custom delete function
					if(!ItemDelFunc || !ItemDelFunc(pBackup->m_pElement))
					{
						delete pBackup->m_pElement;
						pBackup->m_pElement = LNull;
					}

					delete pBackup;

					--m_uNumElements;

					return LTrue;
				}

				pTemp = pTemp->m_pNext;
			}

			return LFalse;
		}

		template<class T> LVoid CSingleLinkedList<T>::RemoveAll(LBool bFreeMemory)
		{
			_LLIST_NODE *pTemp = m_pList; 
			 
			while(pTemp)
			{
				_LLIST_NODE *pNext = pTemp->m_pNext;

				pTemp->m_pNext = LNull;

				if(bFreeMemory && (!ItemDelFunc || !ItemDelFunc(pTemp->m_pElement)))
				{
					delete pTemp->m_pElement;
				}

				pTemp->m_pElement = LNull;

				delete pTemp;

				pTemp = pNext;
			}

			m_pList		= LNull;
			m_pTailItem = LNull;
			m_uNumElements = 0;
		}
	}	// namespace Utils
}	// namespace PIO

#endif //_LLIST_H