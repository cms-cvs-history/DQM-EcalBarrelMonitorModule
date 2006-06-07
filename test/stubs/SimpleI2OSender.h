#ifndef _SimpleI2OSender_h_
#define _SimpleI2OSender_h_

#include "extern/i2o/include/i2o/i2oDdmLib.h"
 
// stl
#include <string>
#include <vector>
#include <deque>
 
// xdaq definitions
#include "xdaq/Application.h"
#include "toolbox/PerformanceMeter.h"
#include "xdaq/WebApplication.h"
                                                                                                                      
// i2o definitions
#include "i2o.h"
#include "i2o/Method.h"
#include "i2o/utils/include/i2o/utils/AddressMap.h"

// I2O Message frame format
#include "interface/shared/include/i2oXFunctionCodes.h"
                                                                                                                      
// toolbox definitions ... from FEDStreamer.cc
#include "toolbox/mem/MemoryPoolFactory.h"
#include "toolbox/task/WorkLoopFactory.h"
#include "toolbox/fsm/FailedEvent.h"
// toolbox definitions
#include "toolbox/mem/CommittedHeapAllocator.h"
#include "toolbox/mem/Reference.h"
#include "toolbox/mem/Pool.h"
#include "toolbox/fsm/FiniteStateMachine.h"
#include "toolbox/task/Action.h"
#include "toolbox/task/WorkLoop.h"
#include "toolbox/math/random.h"
#include "toolbox/rlist.h"
                                                                                                                      
// xdata definitions
#include "xdata/Integer.h"
#include "xdata/UnsignedLong.h"
#include "xdata/Double.h"
#include "xdata/Boolean.h"
#include "xdata/ActionListener.h"
#include "xdata/Vector.h"
                                                                                                                      
                                                                                                                      
//
// cgicc
//
#include "cgicc/CgiDefs.h"
#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"
#include "xgi/Utils.h"
#include "xgi/Input.h"
#include "xgi/Output.h"
#include "xgi/Method.h"


class SimpleI2OSender: public xdaq::WebApplication  
{
	
	public:
	
	XDAQ_INSTANTIATOR();
	
	SimpleI2OSender(xdaq::ApplicationStub * s);
		
	void Default(xgi::Input * in, xgi::Output * out ) throw (xgi::exception::Exception);
	
	void sendMessage(xgi::Input * in, xgi::Output * out ) throw (xgi::exception::Exception);

	void sendAll(xgi::Input * in, xgi::Output * out ) throw (xgi::exception::Exception);

        private:

    
 	/**
 	 * Exported read/write parameter specifiying the size of payload per FED
 	 * per event.
 	 *
 	 * fedPayloadSize is in bytes.
 	 */
 	xdata::UnsignedLong fedPayloadSize_;

    	/**
    	 * The I2O tid of the application to which the RUI pushes super-fragment
    	 * data.
    	 */
    	I2O_TID destinationTid_;

    xdata::String destinationClass_;
    xdata::Integer destinationInstance_;

    	/**
    	 * Exported read/write parameter specifiying the source id of the FED
    	 * fragment which is also the super-fragment.
    	 */
    	xdata::UnsignedLong fedSourceId_;

    	/**
    	 * Exported read-only parameter specifying the event number of the next
    	 * dummy super-fragment to be created.
    	 */
    	xdata::UnsignedLong eventNumber_;

        /**
         * Sleep time in milliseconds between two events
         */
        xdata::UnsignedLong sleepTime_;


toolbox::mem::Reference *generate
(
    toolbox::mem::MemoryPoolFactory *poolFactory,
    toolbox::mem::Pool              *pool,
    const I2O_TID                   initiatorAddress,
    const I2O_TID                   targetAddress,
    const unsigned long             fedSourceId,
    const U32                       eventNumber,
    char                            *bufferChar,
    const unsigned long             bufferSize
)
throw (xcept::Exception);

	std::ifstream dataFile;

};

#endif
