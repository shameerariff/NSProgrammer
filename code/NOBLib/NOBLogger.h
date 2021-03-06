/*
 
 Copyright (C) 2013 Nolan O'Brien
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */

#import <Foundation/Foundation.h>

/**
    @def DLOG(format, ...)
    A convenience macro for logging directly to \c NSLog in \c DEBUG builds. No-op in \c non-DEBUG builds. Log by providing the \c NOBLogLevel. Provide a format string followed by any format arguments.
 */
#if !defined(DEBUG)
#define DLOG(format, ...)     ((void)0)
#else
#define DLOG(format, ...)     NSLog(format,##__VA_ARGS__)
#endif

/**
    @def LOG(lvl, format, ...)
    Log by providing the \c NOBLogLevel. Provide a format string followed by any format arguments.
 */
#define LOG(lvl, format, ...) [NOBLOG writeASync:[NSString stringWithFormat:format, ##__VA_ARGS__] level:lvl]

/**
    @def LOG_HI(format, ...)
    Log using \c NOBLogLevel_High. Provide a format string followed by any format arguments.
 */
#define LOG_HI(format, ...)  LOG(NOBLogLevel_High, format,##__VA_ARGS__)
/**
    @def LOG_MID(format, ...)
    Log using \c NOBLogLevel_Mid. Provide a format string followed by any format arguments.
 */
#define LOG_MID(format, ...) LOG(NOBLogLevel_Mid, format,##__VA_ARGS__)
/**
    @def LOG_LO(format, ...)
    Log using \c NOBLogLevel_Low. Provide a format string followed by any format arguments.
 */
#define LOG_LO(format, ...)  LOG(NOBLogLevel_Low, format,##__VA_ARGS__)

/**
    @def LOG_DBG(format, ...)
    Log at the lowest level in \c DEBUG builds.  No-op on \c non-DEBUG builds. Provide a format string followed by any format arguments.
 */
#ifdef DEBUG
#define LOG_DBG(format, ...) ((void)0)
#else
#define LOG_DBG(format, ...) LOG(NOBLogLevel_Low, format,##__VA_ARGS__)
#endif

/**
    @def NOBLOG
    Helper macro for easy access to the \c NOBLogger class' \c sharedLog
 */
#define NOBLOG [NOBLogger sharedLog]

/**
    @enum kNOBLoggerDefault Value Contants
    @constant kNOBLoggerDefaultRolloverSize
    @constant kNOBLoggerDefaultMaxFiles
    @constant kNOBLoggerDefaultWritesPerFlush
    @constant kNOBLoggerDefaultFilePrefix
 */
FOUNDATION_EXPORT NSUInteger const kNOBLoggerDefaultRolloverSize;     /*!< \c 500 writes, not bytes */
FOUNDATION_EXPORT NSUInteger const kNOBLoggerDefaultMaxFiles;         /*!< \c 10 files */
FOUNDATION_EXPORT NSUInteger const kNOBLoggerDefaultWritesPerFlush;   /*!< \c 10 writes per flush */
FOUNDATION_EXPORT NSString*  const kNOBLoggerDefaultFilePrefix;       /*!< \c \@"log." */

/**
    @enum NOBLogLevel
    Enum of Log Levels that can be set in \c NOBLogger
 */
typedef NS_ENUM(NSUInteger, NOBLogLevel)
{
    NOBLogLevel_Off = 0,    /**< level to indicate logging is off */
    NOBLogLevel_High,       /**< level for important logs (errors, warnings) */
    NOBLogLevel_Mid,        /**< level for un-important logs (status, state changes, information) */
    NOBLogLevel_Low         /**< level for verbose logs that released builds will likely not log (debug logs) */
};

/**
    @class NOBLogger
 
    @par \c NOBLogger is used for logging to disk in a thread safe way.
    @par The \c NOBLogger also logs using a rolling log mechanism so that logging does not overtake the user's disk space.
 */
@interface NOBLogger : NSObject

/** 
    Accessor to the global shared log.
    @par The shared log should not only be used throughout an application but is what the \c NOBLib and \c NOBUILib use for logging.
    @return the globally shared log
    @see setSharedLog:
 */
+ (NOBLogger*) sharedLog;
/**
    Set the globally shared log.
    @param log the \c NOBLogger desired to be used globally as the shared log.
    @see sharedLog
 */
+ (void) setSharedLog:(NOBLogger*)log;

/**
    Same as initWithDirectory:filePrefix:logLevel:writesBeforeRollover:maxFileCount: but with \a prefix, \a writesBeforeRollover and \a fileCount set to default values.
    @see kNOBLoggerDefault Value Contants
    @see initWithDirectory:filePrefix:logLevel:writesBeforeRollover:maxFileCount:
 */
- (instancetype) initWithDirectory:(NSString*)logsDirectory
                          logLevel:(NOBLogLevel)level;
/**
    Initialize the \c NOBLogger
    @param logsDirectory the directory on disk to put the logs as they are generated.  It is recommended that this directory be dedicated for the logs and that no other files be in it.
    @param prefix the file name prefix for all logs.  Pass \c nil for \c kNOBLoggerDefaultFilePrefix.  \a prefix can be an empty string.
    @warning throws \c NSDestinationInvalidException when the \a logsDirectory is invalid
    @warning throws \c NSObjectInaccessibleException when a log file cannot be opened
    @see NOBLogger Defaults
 */
- (instancetype) initWithDirectory:(NSString*)logsDirectory
                        filePrefix:(NSString*)prefix
                          logLevel:(NOBLogLevel)level
              writesBeforeRollover:(NSUInteger)writesBeforeRollover  // UINT32_MAX for unlimited
                      maxFileCount:(NSUInteger)fileCount;

/**
   Initialize the \c NOBLogger with a default \a path and the \a level set to \c NOBLogLevel_Low if \c DEBUG and NOBLogLevel_High otherwise.
   @see initWithDirectory:logLevel:
 */
+ (instancetype) logWithDefaultConfig;

/**
    The current operating log level.  Changing the \c logLevel will change what gets logged to the logs.  Duh!
 */
@property (nonatomic, assign) NOBLogLevel logLevel;
/**
    The number of write calls made to the \c NOBLogger before it is flushed to disk.  The less the logs are flushed, the more efficient the logger, however if the logs are not flushed prior to the app terminating (via crash or normally) they will not be availble on disk for future access.
    @see flush
 */
@property (nonatomic, assign) NSUInteger writesPerFlush;
/**
    The number of write calls made to the \c NOBLogger before the log file rolls over.
    @param writesBeforeRollover provide \c UINT32_MAX for unlimited log files.
    @see logFiles
 */
@property (nonatomic, assign) NSUInteger writesBeforeRollover;
/**
    The number of log files that can be written to disk before the eldest log file is purged.
    @see logFiles
 */
@property (nonatomic, assign) NSUInteger maxFileCount;

/**
    flush the logs to disk.  It is recommended to flush the logs on app exit, either via normal termination or crash.
 */
- (void) flush;

/**
    one of the two write methods for writing a log message.  Writes the log asynchronously.  This method is the default method and adds the provided log message to the \c NOBLogger object's writing queue.
    @note This is the default write method to use.
    @param message the string to write to disk.
    @param level the log level for the message.  The message will be filtered based on the \c NOBLogger object's \a logLevel.
    @see writeSync:level:
 */
- (void) writeASync:(NSString*)message level:(NOBLogLevel)level;
/**
    one of the two write methods for writing a log message.  Writes the log synchronously.  This method adds the provided log message to the \c NOBLogger object's writing queue and wait until is finished being written.
    @note It is recommended that any logging made at shutdown time use this method followed by \c flush.
    @param message the string to write to disk.
    @param level the log level for the message.  The message will be filtered based on the \c NOBLogger object's \a logLevel.
    @see writeASync:level:
    @see flush
 */
- (void) writeSync:(NSString*)message level:(NOBLogLevel)level;  // best for writing out logs at shutdown time

/**
    @return an array of all the current log file paths
 */
- (NSArray*) logFiles;
/**
    @return the path to the directory that contains the logs.
 */
- (NSString*) logDirectoryPath;
/**
    @param maxSizeInBytes minimum of \c 1024 and a maximum of \c UINT32_MAX
    @return the data contained in the tail of all the logs capped with \a maxSizeInBytes
 */
- (NSData*) mostRecentLogs:(NSUInteger)maxSizeInBytes;
/**
    @return the total size of all the log files that this \c NOBLogger encompasses in bytes
*/
- (unsigned long long) totalLogSize;

@end
