//
//  ConfigData.h
//  SFS2X
//
//  Original development by Infosfer Game Technologies Ltd. | http://www.infosfer.com.
//
//  Maintained and developed by A51 Integrated.
//  Copyright 2012 A51 Integrated | http://a51integrated.com. All rights reserved.
//

/**
 * Stores the client configuration data loaded from an external XML file.
 * @see [SmartFox2Client connectWithConfig:]
 */
@interface ConfigData : NSObject {

	
	NSString *_host;
	int _port;
	NSString *_udpHost;
	int _udpPort;
	NSString *_zone;
	BOOL _debug;
	
	int _httpPort;
	BOOL _useBlueBox;
	NSTimeInterval _blueBoxPollingRate;
	
}

@property (strong) NSString *host;
@property (assign) int port;
@property (strong) NSString *udpHost;
@property (assign) int udpPort;
@property (strong) NSString *zone;
@property (assign) BOOL debug;
@property (assign) int httpPort;
@property (assign) BOOL useBlueBox;
@property (assign) NSTimeInterval blueBoxPollingRate;

@end
