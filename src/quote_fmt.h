#ifndef __QUOTE_FMT_H__

#define __QUOTE_FMT_H__

#define quote_fmt_parse	quote_fmtparse

void quote_fmt_quote_description(void);

gchar *quote_fmt_get_buffer(void);
void quote_fmt_init(MsgInfo *info, const gchar *my_quote_str,
		    const gchar *my_body, gboolean my_dry_run);
gint quote_fmtparse(void);
void quote_fmt_scan_string(const gchar *str);
void quote_fmt_reset_vartable(void);
gint quote_fmt_get_cursor_pos(void);

#endif /* __QUOTE_FMT_H__ */
