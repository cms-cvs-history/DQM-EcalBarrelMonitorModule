
#include "interface/evb/include/i2oEVBMsgs.h"

#include "interface/shared/include/frl_header.h"
#include "interface/shared/include/fed_header.h"
#include "interface/shared/include/fed_trailer.h"

#include "SimpleI2OSender.h"
//#include "mySuperFragmentGenerator.h"


//
// provides factory method for instantion of SimpleI2OSender application
//
XDAQ_INSTANTIATOR_IMPL(SimpleI2OSender)

// FIXME to be moved to header file
toolbox::mem::CommittedHeapAllocator* a ;
toolbox::net::URN urn("toolbox-mem-pool", "MyVirtualMemoryPool");
toolbox::mem::Pool* pool ;

SimpleI2OSender::SimpleI2OSender(xdaq::ApplicationStub * s): xdaq::WebApplication(s) 
{	
	// A simple web control interface
	xgi::bind(this,&SimpleI2OSender::Default, "Default");
	xgi::bind(this,&SimpleI2OSender::sendMessage, "sendMessage");
	xgi::bind(this,&SimpleI2OSender::sendAll, "sendAll");

	// create a pool of virtual memory 
	//a = new toolbox::mem::CommittedHeapAllocator(0x300000); // with a committed size of 3MBytes
	a = new toolbox::mem::CommittedHeapAllocator(0x5000000);
	//a = new toolbox::mem::CommittedHeapAllocator(0x30000); // to make it crash soon if buffers are not released
	//toolbox::net::URN urn("toolbox-mem-pool", "MyVirtualMemoryPool");
	pool = toolbox::mem::getMemoryPoolFactory()->createPool(urn, a);

 	destinationClass_    = "RU";
 	destinationInstance_ = getApplicationDescriptor()->getInstance();
        eventNumber_         = 1;
	fedSourceId_         = getApplicationDescriptor()->getInstance();

	getApplicationInfoSpace()->fireItemAvailable("destinationClass", &destinationClass_);
	getApplicationInfoSpace()->fireItemAvailable("destinationInstance", &destinationInstance_);
	getApplicationInfoSpace()->fireItemAvailable("eventNumber", &eventNumber_);
	getApplicationInfoSpace()->fireItemAvailable("fedSourceId", &fedSourceId_);
	getApplicationInfoSpace()->fireItemAvailable("sleepTime", &sleepTime_);


	dataFile.open ("/tmp/RAWDATA.BINARY", ios::binary );
	dataFile.seekg (0, ios::beg);
	// skip 1st event to have LV1 = 1
	unsigned long a=1;
 	dataFile.seekg(2*4,ios::cur);
 	dataFile.read(reinterpret_cast<char *>(&a),sizeof(unsigned long));
 	unsigned long size =  a & 0xffffff ;
 	//ritorna all'inizio dell'evento;
 	dataFile.seekg(-3*4,ios::cur);
 	dataFile.seekg(size*8,ios::cur);

}

void SimpleI2OSender::Default(xgi::Input * in, xgi::Output * out ) throw (xgi::exception::Exception)
{
	*out << cgicc::HTMLDoctype(cgicc::HTMLDoctype::eStrict) << std::endl;
	*out << cgicc::html().set("lang", "en").set("dir","ltr") << std::endl;
	*out << cgicc::title("Send I2O Message") << std::endl;

	xgi::Utils::getPageHeader
		(out, 
		"Simple I2O Sender", 
		getApplicationDescriptor()->getContextDescriptor()->getURL(),
		getApplicationDescriptor()->getURN(),
		"/daq/xgi/images/Application.jpg"
		);

	std::string url = "/";
	url += getApplicationDescriptor()->getURN();
	url += "/sendMessage";	
	*out << cgicc::form().set("method","get").set("action", url).set("enctype","multipart/form-data") << std::endl;
	*out << cgicc::input().set("type", "submit").set("name", "send").set("value", "Send");
	*out << cgicc::p() << std::endl;
	*out << cgicc::form() << std::endl;

	std::string url2 = "/";
	url2 += getApplicationDescriptor()->getURN();
	url2 += "/sendAll";	
	*out << cgicc::form().set("method","get").set("action", url2).set("enctype","multipart/form-data") << std::endl;
	*out << cgicc::input().set("type", "submit").set("name", "send").set("value", "SendAll");
	*out << cgicc::p() << std::endl;
	*out << cgicc::form() << std::endl;

	xgi::Utils::getPageFooter(*out);	
}
	
void SimpleI2OSender::sendMessage(xgi::Input * in, xgi::Output * out ) throw (xgi::exception::Exception)
{
 	     toolbox::mem::Reference* ref = 0;
 	     //static toolbox::mem::Reference* ref = 0;
 	      xdaq::ApplicationDescriptor * destinationDescriptor_ = 0;

	// get Tid of this application
	unsigned long originator = i2o::utils::getAddressMap()->getTid(this->getApplicationDescriptor());
	cout << "SimpleI2OSender::sendMessage: originator (Tid) = " << originator <<endl;


	try
	{
	  // 20060929 Benigno - Fix for xdaq 3.7
	  //destinationDescriptor_ = getApplicationContext()->getApplicationGroup()->getApplicationDescriptor(destinationClass_, destinationInstance_);
	  destinationDescriptor_ = getApplicationContext()->getDefaultZone()->getApplicationDescriptor(destinationClass_, destinationInstance_);

	   // char  * bufferChar = new char [8]; // detector data
	   // bufferChar = "12345678";
	   // unsigned long bufferSize = 8 ;
	   // cout <<"bufferSize= "<<bufferSize << endl;
 	 
	 char  * bufferChar;
	 unsigned long bufferSize =0;	
	 unsigned long a=1;
 	 // si sposta sulla parola che contiene la lunghezza dell'evento e la
 	 // decodifica nella variabile size;
 	 dataFile.seekg(2*4,ios::cur);
 	 dataFile.read(reinterpret_cast<char *>(&a),sizeof(unsigned long));
 	 unsigned long size =  a & 0xffffff ;
	 //size -=1;
	 bufferSize = size * 8; // in words of 64 bits
 	 //ritorna all'inizio dell'evento;
 	 dataFile.seekg(-3*4,ios::cur);
 	 //dataFile.seekg(2*4,ios::cur);
 	 // mette nella variabile bufferChar (di dimensione opportuna)
 	 // il contenuto di un evento come viene dal DCC
 	 bufferChar = new char [8* size];
 	 //ulong * bufferUlong = (ulong *) bufferChar;
 	 dataFile.read (bufferChar,size*8);
 	 // a questo punto bufferChar o bufferUlong puntano ad una zona di memoria	 // di dimensione 8*size contenente l'evento.
 
 	 // Qui mandavamo l'evento al parser e tu potresti trasferirlo alla BU.
 	 //bool SingleEvent = true;
 	 //myParser.parseBuffer(bufferUlong, size*8, SingleEvent);
 
 	 // si libera la memoria dopo il parsing prima di leggere
 	 // l'evento successivo.
 	 //delete  bufferChar;
 	 // nevt++;
                                                                                
	    
	    //if(ref != 0) ref->release();
	    
            //cout << "ref = " << (int *) ref << endl;
	    ref = generate
            (
                toolbox::mem::getMemoryPoolFactory(),            // poolFactory
                toolbox::mem::getMemoryPoolFactory()->findPool(urn),            // pool
                i2o::utils::getAddressMap()->getTid(getApplicationDescriptor()),                    // initiatorAddress
                i2o::utils::getAddressMap()->getTid(destinationDescriptor_),                 // targetAddress
                fedSourceId_.value_, // triggerSourceId
                eventNumber_.value_,      // eventNumber
		bufferChar,          // event data
		bufferSize           // event length in BYTES
            );
            delete bufferChar;

              // The event number of CMS will be 24-bits
              // 2 to the power of 24 = 16777216
              eventNumber_.value_ = (eventNumber_.value_ + 1) % 16777216;
              //cout << "eventNumber_" << eventNumber_ <<endl;

	      //pass the executive that will subsequently route it to the configured peer-transport for sending
	      this->getApplicationContext()->postFrame(ref, getApplicationDescriptor(), destinationDescriptor_);
	
	      //if( ref != 0 ) ref->release();
	      
	}
	catch (toolbox::mem::exception::Exception& e)
	{

	      cout << "Cannot send message"<< endl;
	      XCEPT_RETHROW (xgi::exception::Exception, "Cannot send message", e);
	}
 
		
			
	this->Default(in,out);

}

void SimpleI2OSender::sendAll(xgi::Input * in, xgi::Output * out ) throw (xgi::exception::Exception)
{

	int ev =0;
	while (!dataFile.eof()) {
	  ev++;
	  sendMessage(in, out);
	  sleepmillis(sleepTime_);
	  cout<<"sent event n: "<<ev<<endl;
	  
	}
	cout<<" END OF THE FILE.... "<<endl;
	this->Default(in,out);
}


toolbox::mem::Reference *SimpleI2OSender::generate
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
throw (xcept::Exception)
{
    toolbox::mem::Reference            *bufRef     = 0;
    I2O_MESSAGE_FRAME                  *stdMsg     = 0;
    I2O_PRIVATE_MESSAGE_FRAME          *pvtMsg     = 0;
    I2O_EVENT_DATA_BLOCK_MESSAGE_FRAME *block      = 0;
    unsigned long                      bufSize     = 0;
    char                               *payload    = 0;
    frlh_t                             *frlHeader  = 0;
    //fedh_t                             *fedHeader  = 0;
    //fedt_t                             *fedTrailer = 0;


    bufSize = sizeof(I2O_EVENT_DATA_BLOCK_MESSAGE_FRAME) + // RU builder header
              sizeof(frlh_t)                             + // FRL header
              //sizeof(fedh_t)                             + // FED header
              bufferSize                            ;  //   + // detector data 
              //sizeof(fedt_t);                              // FED trailer

cout <<"sizeof(I2O_PRIVATE_MESSAGE_FRAME)= "<<sizeof(I2O_PRIVATE_MESSAGE_FRAME) << endl;
cout <<"sizeof(I2O_EVENT_DATA_BLOCK_MESSAGE_FRAME)= "<<sizeof(I2O_EVENT_DATA_BLOCK_MESSAGE_FRAME) << endl;
cout <<"sizeof(frlh_t)=                             "<<sizeof(frlh_t) << endl;
//cout <<"sizeof(fedh_t)=                             "<<sizeof(fedh_t) << endl;
cout <<"bufferSize=                                 "<<bufferSize << endl;
//cout <<"sizeof(fedt_t)=                             "<<sizeof(fedt_t) << endl;

    ///////////////////////
    // Allocate a buffer //
    ///////////////////////

    try
    {
        bufRef = poolFactory->getFrame(pool, bufSize);
        bufRef->setDataSize(bufSize);
    }
    catch(xcept::Exception e)
    {
        XCEPT_RETHROW(xcept::Exception,
                      "Failed to allocate I2O_EVM_TRIGGER message", e);
    }


    /////////////////////
    // fill with zeros //
    /////////////////////

    ::memset(bufRef->getDataLocation(), 0, bufSize);


    ////////////////////////
    // Fill in the fields //
    ////////////////////////

    stdMsg = (I2O_MESSAGE_FRAME*)bufRef->getDataLocation();
    pvtMsg = (I2O_PRIVATE_MESSAGE_FRAME*)stdMsg;
    block  = (I2O_EVENT_DATA_BLOCK_MESSAGE_FRAME*)stdMsg;

    stdMsg->MessageSize      = bufSize >> 2;
    stdMsg->InitiatorAddress = initiatorAddress;
    stdMsg->TargetAddress    = targetAddress;
    stdMsg->Function         = I2O_PRIVATE_MESSAGE;
    stdMsg->VersionOffset    = 0;
    stdMsg->MsgFlags         = 0;  // Point-to-point

    pvtMsg->XFunctionCode    = I2O_EVMRU_DATA_READY;
    pvtMsg->OrganizationID   = XDAQ_ORGANIZATION_ID;

    block->eventNumber             = eventNumber;
    block->nbBlocksInSuperFragment = 1;
    block->blockNb                 = 0;

    payload = ((char*)block) + sizeof(I2O_EVENT_DATA_BLOCK_MESSAGE_FRAME);

    frlHeader   = (frlh_t*)(payload);
    //fedHeader   = (fedh_t*)(payload + sizeof(frlh_t));
    //fedTrailer  = (fedt_t*)(payload + sizeof(frlh_t) + sizeof(fedh_t) + bufferSize);
    		
    //char * fedData = (char*) (payload + sizeof(frlh_t) + sizeof(fedh_t)) ;
    char * fedData = (char*) (payload + sizeof(frlh_t)) ;
    memcpy(fedData, bufferChar, bufferSize);	
    	
    frlHeader->source     = 1;
    frlHeader->trigno     = eventNumber;
    frlHeader->segno      = 0;
    frlHeader->reserved   = 0;
    //frlHeader->segsize    = FRL_LAST_SEGM | (sizeof(fedh_t) + bufferSize + sizeof(fedt_t));
    //frlHeader->segsize    = FRL_LAST_SEGM | (sizeof(fedh_t) + bufferSize );
    frlHeader->segsize    = FRL_LAST_SEGM | ( bufferSize );
    frlHeader->reserved_2 = 0;

    //fedHeader->sourceid = fedSourceId;
    //fedHeader->sourceid = ( 1<<8 ) & FED_SOID_MASK;
    //fedHeader->eventid  = 0x50000000 | eventNumber;

    //fedTrailer->conscheck = 0xDEADFACE;
    //fedTrailer->eventsize = 0xA0000000 |
    //                        ((sizeof(fedh_t) + bufferSize + sizeof(fedt_t)) >> 3);

    return bufRef;
}
