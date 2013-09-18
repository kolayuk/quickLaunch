/*
 * FgrObserver.h
 *
 *  Created on: 24.10.2009
 *      Author: Avis
 */

#ifndef FGROBSERVER_H_
#define FGROBSERVER_H_

#include <apgtask.h>
#include <w32std.h>
class MFgrCallBack 
{
public:
	virtual void KeyCaptured(TWsEvent aEvent)=0;
	virtual void FocusChanged(TUid aUid)=0;
};
 
class CFgrObserver : public CActive 
{
public:
	static CFgrObserver* NewL(RWsSession& aWsSession,MFgrCallBack& aCallBack);
	static CFgrObserver* NewLC(RWsSession& aWsSession,MFgrCallBack& aCallBack);
	virtual ~CFgrObserver();
	void CaptureKey(TInt aScancode);
	RWindowGroup    iWg;
	RArray<TInt32> iHandles;
	void Listen();
private:
	CFgrObserver(RWsSession& aWsSession,MFgrCallBack& aCallBack);
	void ConstructL();
	void RunL();
	void DoCancel();
private:
	MFgrCallBack& 	iCallBack;
	RWsSession&     iWsSession; 
};
#endif /* FGROBSERVER_H_ */
