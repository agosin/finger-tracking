//
//  OscSend.h
//  opencvExample
//
//  Created by annie zhang on 1/30/13.
//
//

#ifndef opencvExample_OscSend_h
#define opencvExample_OscSend_h

namespace osc{
    
    void RunSend( unsigned long address, int port , int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x5, int y5);
    void SendPosition(int p, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x5, int y5);
    
    void RunSend( unsigned long address, int port , int x1, int y1, int x2, int y2);
    void SendPosition(int p, int x1, int y1, int x2, int y2);
    
} // namespace osc

#endif /* INCLUDED_OSCSENDTESTS_H */

