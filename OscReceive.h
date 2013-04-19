//
//  OscReceive.h
//  opencvExample
//
//  Created by annie zhang on 1/30/13.
//
//

#ifndef opencvExample_OscReceive_h
#define opencvExample_OscReceive_h

namespace osc{
    
    void RunReceive( int port );
    int listen();
    int oscflag = 0;

} // namespace osc

#endif
