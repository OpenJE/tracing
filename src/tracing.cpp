// OpenJE

#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include "tracing.hpp"

#if defined(TRACING)
namespace tracing {
	static inline const char* TraceToString( Trace trace ) {
		switch ( trace ) {
			case ENTER: return "ENTER";
			case EXIT:  return "EXIT";
			default:    return "UNKNOWN";
		}
	}

	void enter( const Trace trace, const Location location, const char* fmt, va_list args ) {
		time_t t = time( 0 );
		char timebuf[ 20 ];
		strftime( timebuf, sizeof( timebuf ), "%Y-%m-%d %H:%M:%S", localtime( &t ) );

		char header[ 256 ];
		_snprintf( header, sizeof( header ), "[%s] [%s] (%s:%d %s): ", timebuf, TraceToString( trace ), location.file, location.line, location.function );

		char message[ 1024 ];
		_vsnprintf( message, sizeof( message ), fmt, args );

		#if defined( TRACE_TERMINAL )
		fprintf(
			stderr,
			"%s%s%s\n",
			"\x1b[0m",
			header,
			message
		);
		#endif // LOG_TERMINAL

		#if defined( TRACE_FILE )
		static const char* LOG_FILE_PATH = "log.txt";
		FILE* logFile = fopen( LOG_FILE_PATH, "a" );
		if ( logFile ) {
			fprintf( logFile, "%s%s\n", header, message );
			fclose( logFile );
		}
		#endif // LOG_FILE
	} // _LOG

	void exit( const Trace trace, const Location location ) {
		time_t t = time( 0 );
		char timebuf[ 20 ];
		strftime( timebuf, sizeof( timebuf ), "%Y-%m-%d %H:%M:%S", localtime( &t ) );

		char header[ 256 ];
		_snprintf( header, sizeof( header ), "[%s] [%s] (%s:%d %s)", timebuf, TraceToString( trace ), location.file, location.line, location.function );

		#if defined( TRACE_TERMINAL )
		fprintf(
			stderr,
			"%s%s\n",
			"\x1b[0m",
			header
		);
		#endif // LOG_TERMINAL

		#if defined( TRACE_FILE )
		static const char* LOG_FILE_PATH = "log.txt";
		FILE* logFile = fopen( LOG_FILE_PATH, "a" );
		if ( logFile ) {
			fprintf( logFile, "%s\n", header);
			fclose( logFile );
		}
		#endif // LOG_FILE
	} // _LOG

	#define COLOR_RESET "\x1b[0m"
	#define COLOR_INFO  "\x1b[36m" // Cyan
	#define COLOR_WARN  "\x1b[33m" // Yellow
	#define COLOR_ERROR "\x1b[31m" // Red

	static inline const char* LevelToString( Level level ) {
		switch ( level ) {
			case INFO:  return "INFO";
			case WARN:  return "WARN";
			case ERR:   return "ERROR";
			default:    return "UNKNOWN";
		}
	}

	static inline const char* LevelToColor( Level level ) {
		switch ( level ) {
			case INFO:  return COLOR_INFO;
			case WARN:  return COLOR_WARN;
			case ERR:   return COLOR_ERROR;
			default:    return COLOR_RESET;
		}
	}

	void log( const Level level, const Location location, const char* fmt, va_list args ) {
		time_t t = time( 0 );
		char timebuf[ 20 ];
		strftime( timebuf, sizeof( timebuf ), "%Y-%m-%d %H:%M:%S", localtime( &t ) );

		char header[ 256 ];
		_snprintf( header, sizeof( header ), "[%s] [%s] (%s:%d %s): ", timebuf, LevelToString( level ), location.file, location.line, location.function );

		char message[ 1024 ];
		_vsnprintf( message, sizeof( message ), fmt, args );

		#if defined( TRACE_TERMINAL )
		fprintf(
			stderr,
			"%s%s%s%s\n",
			LevelToColor( level ),
			header,
			COLOR_RESET,
			message
		);
		#endif // LOG_TERMINAL

		#if defined( TRACE_FILE )
		static const char* LOG_FILE_PATH = "log.txt";
		FILE* logFile = fopen( LOG_FILE_PATH, "a" );
		if ( logFile ) {
			fprintf( logFile, "%s%s\n", header, message );
			fclose( logFile );
		}
		#endif // LOG_FILE
	} // _LOG
} // namespace tracing
#endif // TRACING
