/**
 * \file formatter.hpp Formats a topic into human-readable, compiler lookalike format.
 */

#ifndef __LOGOG_FORMATTER_HPP__
#define __LOGOG_FORMATTER_HPP__

namespace logog
{
/** A helper object for generating a current timestamp as a string. */
class TimeStamp : public Object
{
public:
	/** Returns a pointer to a string representing the current time.  Note!  This pointer is only
	 ** valid while this object is valid -- if you destroy this object, this pointer is no longer
	 ** valid.
	 */
	const char *Get(const char *fmt=LOGOG_DEFAULT_TIME_FORMAT);

protected:
	char cTimeString[ LOGOG_TIME_STRING_MAX ]; 
};

/** Converts a topic into a human-readable string for printing or otherwise rendering to a target. */
class Formatter : public Object
{
public:
	Formatter();

    /** Causes this formatter to format a topic into its own m_sMessageBuffer field, and thence to
     ** return a reference to that string.  This function must be written to be efficient; it will be called
     ** for every logging operation.  It is strongly recommended not to allocate or free memory in this function.
     **/
    virtual LOGOG_STRING &Format( const Topic &topic, const Target &target ) = 0;

	/** Causes the time of day to be rendered, if it needs to be rendered.  This function is only supported on
	 ** ANSI builds, not Unicode, as the underlying functions are ANSI only.
	 */
	virtual void RenderTimeOfDay();

	/** In the base case, this function calls GetTopicFlags() on the provided 
	 ** topic in order to figure out which fields this formatter should render.
	 ** However, subclasses of the Formatter class can override this function in order
	 ** to change the default fields that the topic wants to be rendered.  For example,
	 ** you can turn off the TOPIC_LINE_NUMBER_FLAG and the TOPIC_FILE_NAME_FLAG
	 ** in order to disable these fields from being rendered in your own Formatter
	 ** subclass.
	 ** \param topic The topic whose flags are to be determined
	 ** \return The set of flags representing the topics that really need to be rendered.
	 **/
	virtual TOPIC_FLAGS GetTopicFlags( const Topic &topic );

	/** Should this formatter render the current time of day? */
	bool GetShowTimeOfDay() const;

	/** Sets whether this formatter renders the current time of day. Time of day is not rendered by default. 
     ** This function is only supported on ANSI builds, not Unicode, as the underlying functions are ANSI only.
     **/
	void SetShowTimeOfDay(bool val);

#ifndef LOGOG_UNICODE
	/** Sets the format string used to render the current time of day. 
     ** Uses the same format specifier as strftime, check your compiler's documentation for details.
     ** This function is only supported on ANSI builds, not Unicode, as the underlying functions are ANSI only.
     **/
	void SetTimeOfDayFormat( const char *fmt );
#endif

protected:
    const LOGOG_CHAR *ErrorDescription( const LOGOG_LEVEL_TYPE level );

    LOGOG_STRING m_sMessageBuffer;
    LOGOG_STRING m_sIntBuffer;

	bool m_bShowTimeOfDay;

#ifndef LOGOG_UNICODE
    char m_TimeOfDayFormat[LOGOG_TIME_FORMAT_MAX];
#endif

};

class FormatterGCC : public Formatter
{
public:
    virtual LOGOG_STRING &Format( const Topic &topic, const Target &target );

};

class FormatterMSVC : public Formatter
{
public:
    virtual LOGOG_STRING &Format( const Topic &topic, const Target &target );
};

extern Formatter &GetDefaultFormatter();
extern void DestroyDefaultFormatter();

}

#endif // __LOGOG_FORMATTER_HPP_
