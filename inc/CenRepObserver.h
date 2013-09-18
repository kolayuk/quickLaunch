/*
 ============================================================================
 Name		: CenRepObserver.h
 Author	  : Usanov-Kornilov Nikolay (aka Kolay)
 Version	 : 1.0
 Copyright   : 
 Contacts:
 kolayuk@mail.ru
 http://kolaysoft.ru

 (c) KolaySoft, 2010

 Description : CCenRepObserver declaration
 ============================================================================
 */

#ifndef CENREPOBSERVER_H
#define CENREPOBSERVER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

class CRepository; //forward declaration
//a observer interface for profile change
class MCRCallBack
       {
       public:
               virtual void ValueChanged(TInt aValue) = 0;
        };
//These active objects observe the changing of the profile
//and notify the observer when the change has occured
class CCenRepObserver : public CActive
       {
       public:
               static CCenRepObserver* NewL( MCRCallBack& aObserver);
               static CCenRepObserver* NewLC(MCRCallBack& aObserver);
                void StartObservingL();
                void StopObserving();
                virtual ~CCenRepObserver();
                void RunL();
                void RunErrorL(TInt aError);
                void DoCancel();
                CCenRepObserver(MCRCallBack& aObserver);
                void ConstructL();
       private:
              CRepository* iRepository;
              TUint32 iKey;
              TUid iUid;
              MCRCallBack& iObserver; //class using the observer
        };
#endif // CENREPOBSERVER_H
