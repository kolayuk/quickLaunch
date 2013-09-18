/*
 * const.h
 *
 *  Created on: 15.03.2011
 *      Author: mvideo
 */

#ifndef CONST_H_
#define CONST_H_

const TInt VER_MAJOR=1;
const TInt VER_MINOR=5;
const TInt VER_BUILD=0;


const TSize KButtonSize=TSize(70,70);
const TInt KBetween=1;
#ifdef _DEBUG
const TInt KHidePeriod=300000;
#else
const TInt KHidePeriod=5000;
#endif
const TInt KMinPressPeriod=400;
const TInt KMaxPressPeriod=2000;

const TInt KCheckInactivityPeriod=5000;
_LIT(KSettingPath,"C:\\system\\apps\\quickLaunch\\settings.ini");
_LIT(KFirstStartFile,"C:\\system\\apps\\quickLaunch\\firststart");
_LIT(KLogsFile,"C:\\logs\\quickLaunch\\log.txt");
_LIT(KExceptionsPath,"C:\\system\\apps\\quickLaunch\\exceptions.ini");
_LIT(KPeriodPath,"C:\\system\\apps\\quickLaunch\\delay.ini");
const TUint32 KApplicationWork=1;
const TUint32 KAutoStart=1;
const TUint32 KPositionLR=0;
const TUint32 KApp1=0;
const TUint32 KApp2=0;
const TUint32 KApp3=0;
const TUint32 KApp4=0;
const TUint32 KApp5=0;

#endif /* CONST_H_ */
