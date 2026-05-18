// OpenJE

#ifndef TRACING_H
#define TRACING_H

#include <stdarg.h>

namespace tracing {
	struct Location {
		const char* file;
		const int line;
		const char* function;
		Location( const char* file, const int line, const char* function ) : file( file ), line( line ), function( function ) {}
	};

	#define LOCATION Location( __FILE__, __LINE__, __FUNCTION__ )

	enum Trace {
		ENTER,
		EXIT
	};

	void enter( const Trace trace, const Location location, const char* fmt, va_list args );
	void exit( const Trace trace, const Location location );

	class Instrument {
		private:
			#if defined( TRACING )
			const Location location;
			#endif // TRACING
		public:
			Instrument( const Location location, const char* fmt, ... )
				#if defined( TRACING )
				: location( location )
				#endif // TRACING
			{
				#if defined( TRACING )
				va_list args;
				va_start( args, fmt );
				enter( ENTER, location, fmt, args );
				va_end( args );
				#endif // TRACING
			}
			~Instrument() {
				#if defined( TRACING )
				exit( EXIT, location );
				#endif // TRACING
			}
	};

	#define instrument Instrument scope

	enum Level {
		INFO,
		WARN,
		ERR
	};

	void log( const Level level, const Location location, const char* fmt, va_list args );

	inline void info( const Location location, const char* fmt, ... ) {
		#if defined( TRACING )
		va_list args;
		va_start( args, fmt );
		log( INFO, location, fmt, args );
		va_end( args );
		#endif // TRACING
	}

	inline void warn( const Location location, const char* fmt, ... ) {
		#if defined( TRACING )
		va_list args;
		va_start( args, fmt );
		log( WARN, location, fmt, args );
		va_end( args );
		#endif // TRACING
	}

	inline void error( const Location location, const char* fmt, ... ) {
		#if defined( TRACING )
		va_list args;
		va_start( args, fmt );
		log( ERR, location, fmt, args );
		va_end( args );
		#endif // TRACING
	}
} // namespace tracing
#endif // TRACING_H
