#ifndef MATCHER_H

#define MATCHER_H

#include <sys/types.h>
#include <regex.h>
#include <glib.h>
#include "procmsg.h"

struct _MatcherProp {
	int matchtype;
	int criteria;
	gchar * header;
	gchar * expr;
	int value;
	regex_t * preg;
	int error;
	gboolean result;
};

typedef struct _MatcherProp MatcherProp;

struct _MatcherList {
	GSList * matchers;
	gboolean bool_and;
};

typedef struct _MatcherList MatcherList;


enum {
	/* match */
	MATCHCRITERIA_ALL,
	MATCHCRITERIA_UNREAD, MATCHCRITERIA_NOT_UNREAD,
	MATCHCRITERIA_NEW, MATCHCRITERIA_NOT_NEW,
	MATCHCRITERIA_MARKED, MATCHCRITERIA_NOT_MARKED,
	MATCHCRITERIA_DELETED, MATCHCRITERIA_NOT_DELETED,
	MATCHCRITERIA_REPLIED, MATCHCRITERIA_NOT_REPLIED,
	MATCHCRITERIA_FORWARDED, MATCHCRITERIA_NOT_FORWARDED,
	MATCHCRITERIA_SUBJECT, MATCHCRITERIA_NOT_SUBJECT,
	MATCHCRITERIA_FROM, MATCHCRITERIA_NOT_FROM,
	MATCHCRITERIA_TO, MATCHCRITERIA_NOT_TO,
	MATCHCRITERIA_CC, MATCHCRITERIA_NOT_CC,
	MATCHCRITERIA_TO_OR_CC, MATCHCRITERIA_NOT_TO_AND_NOT_CC,
	MATCHCRITERIA_AGE_GREATER, MATCHCRITERIA_AGE_LOWER,
	MATCHCRITERIA_NEWSGROUPS, MATCHCRITERIA_NOT_NEWSGROUPS,
	MATCHCRITERIA_INREPLYTO, MATCHCRITERIA_NOT_INREPLYTO,
	MATCHCRITERIA_REFERENCES, MATCHCRITERIA_NOT_REFERENCES,
	MATCHCRITERIA_SCORE_GREATER, MATCHCRITERIA_SCORE_LOWER,
	MATCHCRITERIA_HEADER, MATCHCRITERIA_NOT_HEADER,
	MATCHCRITERIA_HEADERS_PART, MATCHCRITERIA_NOT_HEADERS_PART,
	MATCHCRITERIA_MESSAGE, MATCHCRITERIA_NOT_MESSAGE,
	MATCHCRITERIA_BODY_PART, MATCHCRITERIA_NOT_BODY_PART,
	MATCHCRITERIA_EXECUTE, MATCHCRITERIA_NOT_EXECUTE,
	MATCHCRITERIA_SCORE_EQUAL,
	/* match type */
	MATCHTYPE_MATCHCASE,
	MATCHTYPE_MATCH,
	MATCHTYPE_REGEXPCASE,
	MATCHTYPE_REGEXP,
	/* actions */
	MATCHACTION_SCORE,
	MATCHACTION_EXECUTE,
	MATCHACTION_MOVE,
	MATCHACTION_COPY,
	MATCHACTION_DELETE,
	MATCHACTION_MARK,
	MATCHACTION_UNMARK,
	MATCHACTION_MARK_AS_READ,
	MATCHACTION_MARK_AS_UNREAD,
	MATCHACTION_FORWARD,
	MATCHACTION_FORWARD_AS_ATTACHMENT,
	MATCHACTION_COLOR,
	MATCHACTION_BOUNCE,
	/* boolean operations */
	MATCHERBOOL_OR,
	MATCHERBOOL_AND,
};

#if 0
enum {
	/* msginfo flags */
	MATCHING_ALL,
	MATCHING_UNREAD,
	MATCHING_NOT_UNREAD,
	MATCHING_NEW,
	MATCHING_NOT_NEW,
	MATCHING_MARKED,
	MATCHING_NOT_MARKED,
	MATCHING_DELETED,
	MATCHING_NOT_DELETED,
	MATCHING_REPLIED,
	MATCHING_NOT_REPLIED,
	MATCHING_FORWARDED,
	MATCHING_NOT_FORWARDED,

	/* msginfo headers */
	MATCHING_SUBJECT,
	MATCHING_NOT_SUBJECT,
	MATCHING_FROM,
	MATCHING_NOT_FROM,
	MATCHING_TO,
	MATCHING_NOT_TO,
	MATCHING_CC,
	MATCHING_NOT_CC,
	MATCHING_TO_OR_CC,
	MATCHING_NOT_TO_AND_NOT_CC,
	MATCHING_AGE_GREATER,
	MATCHING_AGE_LOWER,
	MATCHING_NEWSGROUPS,
	MATCHING_NOT_NEWSGROUPS,
	MATCHING_INREPLYTO,
	MATCHING_NOT_INREPLYTO,
	MATCHING_REFERENCES,
	MATCHING_NOT_REFERENCES,
	MATCHING_SCORE_GREATER,
	MATCHING_SCORE_LOWER,
	MATCHING_SCORE_EQUAL,

	/* file content */
	MATCHING_HEADER,
	MATCHING_NOT_HEADER,
	MATCHING_MESSAGE,
	MATCHING_NOT_MESSAGE,
	MATCHING_HEADERS_PART,
	MATCHING_NOT_HEADERS_PART,
	MATCHING_BODY_PART,
	MATCHING_NOT_BODY_PART,
	MATCHING_EXECUTE,
	MATCHING_NOT_EXECUTE,

	/* scoring */
	MATCHING_SCORE,

	/* filtering */
	MATCHING_ACTION_MOVE,
	MATCHING_ACTION_COPY,
	MATCHING_ACTION_DELETE,
	MATCHING_ACTION_MARK,
	MATCHING_ACTION_MARK_AS_READ,
	MATCHING_ACTION_UNMARK,
	MATCHING_ACTION_MARK_AS_UNREAD,
	MATCHING_ACTION_FORWARD,
	MATCHING_ACTION_FORWARD_AS_ATTACHMENT,
	MATCHING_ACTION_COLOR,
	/* MATCHING_ACTION_EXECUTE, */

	MATCHING_MATCH,
	MATCHING_REGEXP,
	MATCHING_MATCHCASE,
	MATCHING_REGEXPCASE
};
#endif

gchar * get_matchparser_tab_str(gint id);
MatcherProp * matcherprop_new(gint criteria, gchar * header,
			      gint matchtype, gchar * expr,
			      int age);
void matcherprop_free(MatcherProp * prop);
MatcherProp * matcherprop_parse(gchar ** str);

gboolean matcherprop_match(MatcherProp * prop, MsgInfo * info);

MatcherList * matcherlist_new(GSList * matchers, gboolean bool_and);
void matcherlist_free(MatcherList * cond);
MatcherList * matcherlist_parse(gchar ** str);

gboolean matcherlist_match(MatcherList * cond, MsgInfo * info);

gint matcher_parse_keyword(gchar ** str);
gint matcher_parse_number(gchar ** str);
gboolean matcher_parse_boolean_op(gchar ** str);
gchar * matcher_parse_regexp(gchar ** str);
gchar * matcher_parse_str(gchar ** str);
gchar * matcher_unescape_str(gchar *str);
gchar * matcherprop_to_string(MatcherProp * matcher);
gchar * matcherlist_to_string(MatcherList * matchers);
gchar * matching_build_command(gchar * cmd, MsgInfo * info);


void prefs_matcher_read_config(void);
void prefs_matcher_write_config(void);


#endif
