#include <public/ccguidance_iface_v1.h>

// ***********************************************************************

// class EDROOM_CTX_Top_0

// ***********************************************************************



	// CONSTRUCTORS***********************************************

CCGuidance::EDROOM_CTX_Top_0::EDROOM_CTX_Top_0(CCGuidance &act,
	 CDTMList & EDROOMpVarVCurrentTMList,
	 Pr_Time & EDROOMpVarVNextTimeout,
	 CEDROOMPOOLCDTMList & EDROOMpPoolCDTMList ):

	EDROOMcomponent(act),
	Msg(EDROOMcomponent.Msg),
	MsgBack(EDROOMcomponent.MsgBack),
	GuidanceCtrl(EDROOMcomponent.GuidanceCtrl),
	TMChannelCtrl(EDROOMcomponent.TMChannelCtrl),
	GuidanceTimer(EDROOMcomponent.GuidanceTimer),
	VCurrentTMList(EDROOMpVarVCurrentTMList),
	VNextTimeout(EDROOMpVarVNextTimeout),
	EDROOMPoolCDTMList(EDROOMpPoolCDTMList)
{
}

CCGuidance::EDROOM_CTX_Top_0::EDROOM_CTX_Top_0(EDROOM_CTX_Top_0 &context):

	EDROOMcomponent(context.EDROOMcomponent),
	Msg(context.Msg),
	MsgBack(context.MsgBack),
	GuidanceCtrl(context.GuidanceCtrl),
	TMChannelCtrl(context.TMChannelCtrl),
	GuidanceTimer(context.GuidanceTimer),
	VCurrentTMList(context.VCurrentTMList),
	VNextTimeout(context.VNextTimeout),
	EDROOMPoolCDTMList(context.EDROOMPoolCDTMList )
{

}

	// EDROOMSearchContextTrans********************************************

bool CCGuidance::EDROOM_CTX_Top_0::EDROOMSearchContextTrans(
			TEDROOMTransId &edroomCurrentTrans)
			{

	bool edroomValidMsg=false; 

	 switch(Msg->signal)
	{

		 case ( EDROOMSignalDestroy ): 

			 edroomValidMsg=true;
			 edroomCurrentTrans.distanceToContext = 0 ;
			 edroomCurrentTrans.localId = -1 ;
			 break;

	}

	return(edroomValidMsg);

}

	// User-defined Functions   ****************************

void	CCGuidance::EDROOM_CTX_Top_0::FDoGuidance()

{
   //Define absolute time
  Pr_Time time;
VNextTimeout+= Pr_Time(0,100000);
time=VNextTimeout;
PUSService129::GuidanceControl();
   //Program absolute timer 
   GuidanceTimer.InformAt( time ); 
}



void	CCGuidance::EDROOM_CTX_Top_0::FExecGuidance_TC()

{
   //Handle Msg->data
  CDTCHandler & varSGUIDANCE_TC = *(CDTCHandler *)Msg->data;
CDEventList TCExecEventList; 
PUS_GuidanceTCExecutor::ExecTC(varSGUIDANCE_TC,VCurrentTMList,TCExecEventList); 

}



void	CCGuidance::EDROOM_CTX_Top_0::FInitGuidance()

{
   //Define absolute time
  Pr_Time time;
time.GetTime(); // Get current monotonic time
time+=Pr_Time(0,100000);
//program timer for first time:
VNextTimeout=time;
//no init
   //Program absolute timer 
   GuidanceTimer.InformAt( time ); 
}



void	CCGuidance::EDROOM_CTX_Top_0::FInvokeTxTMList()

{
   //Allocate data from pool
  CDTMList * pSTxTM_Data = EDROOMPoolCDTMList.AllocData();
	*pSTxTM_Data=VCurrentTMList;
VCurrentTMList.Clear();
   //Invoke synchronous communication 
   MsgBack=TMChannelCtrl.invoke(STxTM,pSTxTM_Data,&EDROOMPoolCDTMList); 
}



	//********************************** Pools *************************************

	//CEDROOMPOOLCDTMList

CCGuidance::EDROOM_CTX_Top_0::CEDROOMPOOLCDTMList::CEDROOMPOOLCDTMList(
			TEDROOMUInt32 elemCount,CDTMList* pMem,bool * pMemMarks):
				CEDROOMProtectedMemoryPool(elemCount, pMem, pMemMarks,
					sizeof(CDTMList))
{
}

CDTMList *	CCGuidance::EDROOM_CTX_Top_0::CEDROOMPOOLCDTMList::AllocData()
{
	return(CDTMList*)CEDROOMProtectedMemoryPool::AllocData();
}



// ***********************************************************************

// class EDROOM_SUB_Top_0

// ***********************************************************************



	// CONSTRUCTOR*************************************************

CCGuidance::EDROOM_SUB_Top_0::EDROOM_SUB_Top_0 (CCGuidance&act
	,CEDROOMMemory *pEDROOMMemory):
		EDROOM_CTX_Top_0(act,
			VCurrentTMList,
			VNextTimeout,
			EDROOMPoolCDTMList),
		EDROOMPoolCDTMList(
			10, pEDROOMMemory->poolCDTMList,
			pEDROOMMemory->poolMarkCDTMList)
{

}

	//***************************** EDROOMBehaviour ********************************

void CCGuidance::EDROOM_SUB_Top_0::EDROOMBehaviour()
{

	TEDROOMTransId edroomCurrentTrans;

	//Behaviour starts from Init State

	edroomCurrentTrans = EDROOMIArrival();

	do
	{

		//Take next transition

		switch(edroomCurrentTrans.localId)
		{

			//Next Transition is Init
			case (Init):
				//Execute Action 
				FInitGuidance();
				//Next State is ready
				edroomNextState = ready;
				break;
			//Next Transition is ExecTC
			case (ExecTC):
				//Msg->Data Handling 
				FExecGuidance_TC();
				//Invoke Synchronous Message 
				FInvokeTxTMList();
				//Next State is ready
				edroomNextState = ready;
				break;
			//Next Transition is DoGuidance
			case (DoGuidance):
				//Execute Action 
				FDoGuidance();
				//Next State is ready
				edroomNextState = ready;
				break;
		}

		//Entry into the Next State 
		switch(edroomNextState)
		{

				//Go to the state I
			case (I):
				//Arrival to state I
				edroomCurrentTrans=EDROOMIArrival();
				break;

				//Go to the state ready
			case (ready):
				//Arrival to state ready
				edroomCurrentTrans=EDROOMreadyArrival();
				break;

		}

		edroomCurrentState=edroomNextState;

	}while(Msg->signal != EDROOMSignalDestroy);

}



	// Context Init**********************************************

void CCGuidance::EDROOM_SUB_Top_0::EDROOMInit()
{

edroomCurrentState=I;

}



//	 ***********************************************************************

//	 SubState I

//	 ***********************************************************************



TEDROOMTransId CCGuidance::EDROOM_SUB_Top_0::EDROOMIArrival()
{

	TEDROOMTransId edroomCurrentTrans;

	//Next transition is  Init
	edroomCurrentTrans.localId = Init;
	edroomCurrentTrans.distanceToContext = 0;
	return(edroomCurrentTrans);

}



	// ***********************************************************************

	// Leaf SubState  ready

	// ***********************************************************************



TEDROOMTransId CCGuidance::EDROOM_SUB_Top_0::EDROOMreadyArrival()
{

	TEDROOMTransId edroomCurrentTrans;

	bool edroomValidMsg=false;

	do
	{

		EDROOMNewMessage ();

		switch(Msg->signal)
		{

			case (SGUIDANCE_TC): 

				 if (*Msg->GetPInterface() == GuidanceCtrl)
				{

					//Next transition is  ExecTC
					edroomCurrentTrans.localId= ExecTC;
					edroomCurrentTrans.distanceToContext = 0;
					edroomValidMsg=true;
				 }

				break;

			case (EDROOMSignalTimeout): 

				 if (*Msg->GetPInterface() == GuidanceTimer)
				{

					//Next transition is  DoGuidance
					edroomCurrentTrans.localId= DoGuidance;
					edroomCurrentTrans.distanceToContext = 0;
					edroomValidMsg=true;
				 }

				break;

		};

		if (false == edroomValidMsg)
		{
			 edroomValidMsg = EDROOMSearchContextTrans(edroomCurrentTrans);

		}

	} while (false == edroomValidMsg);

	return(edroomCurrentTrans);

}



