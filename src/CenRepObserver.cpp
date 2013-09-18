/*
 ============================================================================
 Name		: CenRepObserver.cpp
 Author	  : Usanov-Kornilov Nikolay (aka Kolay)
 Version	 : 1.0
 Copyright   : 
 Contacts:
 kolayuk@mail.ru
 http://kolaysoft.ru

 (c) KolaySoft, 2010

 Description : CCenRepObserver implementation
 ============================================================================
 */

#include <centralrepository.h>
#include <CenRepObserver.h>

CCenRepObserver* CCenRepObserver::NewLC(MCRCallBack& aCB)
	{
	CCenRepObserver* self = new (ELeave) CCenRepObserver(aCB);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CCenRepObserver* CCenRepObserver::NewL(MCRCallBack& aCB)
	{
	CCenRepObserver* self = CCenRepObserver::NewLC(aCB);
	CleanupStack::Pop(); // self;
	return self;
	}

//trivial NewL and NewLC have been omitted.
void CCenRepObserver::ConstructL()
        {
        iUid = TUid::Uid(0x101f8765); //observe profile engine to
        iKey = 0x1; //see which profile is active
        iRepository = CRepository::NewL( iUid );
        CActiveScheduler::Add(this);
         }
CCenRepObserver::CCenRepObserver(MCRCallBack& aObserver) : iObserver(aObserver), CActive (EPriorityHigh)
         {
         //do nothing
         }
CCenRepObserver::~CCenRepObserver()
         {
         Cancel(); //first cancel because iRepository is used there
         delete iRepository;
         iRepository=NULL;
         }
void CCenRepObserver::StartObservingL()
         {
         if( IsActive() )
         {
         return; //do nothing if allready observing
         }
         User::LeaveIfError(
                        iRepository->NotifyRequest( iKey, iStatus ) );
         SetActive();
          }
void CCenRepObserver::StopObserving()
          {
          Cancel();
          }
void CCenRepObserver::DoCancel()
          {
          iRepository->NotifyCancel(iKey);
          }
void CCenRepObserver::RunL()
          {
          TInt value = 0;
          TInt error = iRepository->Get( iKey, value);
          //always check the error code
          if( error == KErrNone )
                 {
                 //observer received the profile value as integer
                 //this happens even if the value hasn�t really changed
                 //some application may have set the same value again
                 iObserver.ValueChanged(value);
                 }
          // Re-subscribe
          error = iRepository->NotifyRequest( iKey, iStatus );
          if( error == KErrNone )
               {
               //
               SetActive();
                }
          else
                {
                //todo: error handling
                }
           }

