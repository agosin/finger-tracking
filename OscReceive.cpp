//
//  OscReceive.cpp
//  opencvExample
//
//  Created by annie zhang on 1/30/13.
//
//

#include "OscReceive.h"

#include <string.h>
#include <iostream>

#include "osc/OscReceivedElements.h"

#include "ip/UdpSocket.h"
#include "osc/OscPacketListener.h"


namespace osc{
    
    class OscReceiveTestPacketListener : public OscPacketListener{

    protected:
        
        void ProcessMessage( const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint )
        {
            // a more complex scheme involving std::map or some other method of
            // processing address patterns could be used here
            // (see MessageMappingOscPacketListener.h for example). however, the main
            // purpose of this example is to illustrate and test different argument
            // parsing methods
            
            try {
                // argument stream, and argument iterator, used in different
                // examples below.
                                
                if( strcmp( m.AddressPattern(), "/final" ) == 0 ){
                    
                    ReceivedMessageArgumentStream args = m.ArgumentStream();
             //       ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
                    
                    int32 a1;
                    args >> a1 >> osc::EndMessage;
                    
                    std::cout << "received '/test1' message with arguments: "
                    << a1 << "\n";
                    oscflag = 1;
                }
                
            }catch( Exception& e ){
                std::cout << "error while parsing message: "
                << m.AddressPattern() << ": " << e.what() << "\n";
            }
        }
    };
    
    void RunReceive( int port )
    {
        osc::OscReceiveTestPacketListener listener;
        UdpListeningReceiveSocket s(
                                    IpEndpointName( IpEndpointName::ANY_ADDRESS, port ),
                                    &listener );
        
        std::cout << "listening for input on port " << port << "...\n";
        std::cout << "press ctrl-c to end\n";
        
        s.RunUntilSigInt();
        
        std::cout << "finishing.\n";
    }
    
    int listen()
    {
        
        int port = 6666;
        
        RunReceive( port );
        
        return 0;
    }

    
} // namespace osc




