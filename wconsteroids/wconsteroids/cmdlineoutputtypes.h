#ifndef	COMMANDLINEOUTPUTTYPES_H
#define COMMANDLINEOUTPUTTYPES_H

typedef enum {
	Default = 0x00,
	WordCount = 0x01,
	CharCount = 0x02,
	CodeCount = 0x04,
	CommentCount = 0x08,
	WhiteSpaceCount = 0x0F,
	TotalCount = 0x10,
	Percentages = 0x20
} CmdLineOutputTypes;

#endif // COMMANDLINEOUTPUTTYPES_H
