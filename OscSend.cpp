//
//  OscSend.cpp
//  opencvExample
//
//  Created by annie zhang on 1/30/13.
//
//

#include "OscSend.h"

#include <iostream>
#include <string.h>

#include "osc/OscOutboundPacketStream.h"

#include "ip/UdpSocket.h"
#include "ip/IpEndpointName.h"

#define IP_MTU_SIZE 1536

namespace osc{
    
    void RunSend( const IpEndpointName& host , int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x5, int y5)
    {
        char buffer[IP_MTU_SIZE];
        osc::OutboundPacketStream p( buffer, IP_MTU_SIZE );
        UdpTransmitSocket socket( host );
        
        p.Clear();
        p << osc::BeginMessage( "/pos" )
        << x1 << y1 << x2 << y2 << x3 << y3 << x4 << y4 << x5 << y5 << osc::EndMessage;
        socket.Send( p.Data(), p.Size() );
    }
    
    void SendPosition(int p, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x5, int y5)
    {
        char *hostName = "localhost";
        int port = 7000;
        if (p==1)
            port = 8000;
        if (p==2)
            port = 9000;
        if (p==3)
            port = 7777;
        
        IpEndpointName host( hostName, port );
        
        char hostIpAddress[ IpEndpointName::ADDRESS_STRING_LENGTH ];
        host.AddressAsString( hostIpAddress );
        
        RunSend( host , x1, y1, x2, y2, x3, y3, x4, y4, x5, y5);
    }
    
    void RunSend( const IpEndpointName& host , int x1, int y1, int x2, int y2)
    {
        char buffer[IP_MTU_SIZE];
        osc::OutboundPacketStream p( buffer, IP_MTU_SIZE );
        UdpTransmitSocket socket( host );
        
        p.Clear();
        
        p << osc::BeginMessage( "/test1" )
        << x1 << y1 << x2 << y2 << osc::EndMessage;
        socket.Send( p.Data(), p.Size() );
    }
    
    void SendPosition(int p, int x1, int y1, int x2, int y2)
    {
        char *hostName = "143.215.49.28";
        
        int port = 7000;
        if (p==1)
            port = 8000;
        if (p==2)
            port = 9000;
        if (p==3)
            port = 7777;
        if (p==4)
            port = 8888;
        
        IpEndpointName host(hostName, port );
        
        char hostIpAddress[ IpEndpointName::ADDRESS_STRING_LENGTH ];
        host.AddressAsString( hostIpAddress );
        
        RunSend( host , x1, y1, x2, y2);
    }

}

