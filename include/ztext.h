/******************************************************************************
 *
 * ztext.h -	class zText: a module driver supporting compressed
 *		Bibles with common 2 byte size entries
 *
 * $Id: ztext.h 3786 2020-08-30 11:35:14Z scribe $
 *
 * Copyright 1996-2013 CrossWire Bible Society (http://www.crosswire.org)
 *	CrossWire Bible Society
 *	P. O. Box 2528
 *	Tempe, AZ  85280-2528
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */

#ifndef ZTEXT_H
#define ZTEXT_H

#include <zverse.h>
#include <swtext.h>

#include <defs.h>


SWORD_NAMESPACE_START

/*** SWModule implementation for compressed modules
* This class handles compressed modules.
* It should not be used in frontends, unless you are doing very special things.
*/
class SWDLLEXPORT zText:public zVerse, public SWText {

private:
	VerseKey *lastWriteKey;
	bool sameBlock(VerseKey * lastWriteKey, VerseKey * key);
	int blockType;

public:
	zText(const char *ipath, const char *iname = 0, const char *idesc = 0,
			int blockType = CHAPTERBLOCKS, SWCompress *icomp = 0,
			SWDisplay *idisp = 0, SWTextEncoding encoding = ENC_UNKNOWN,
			SWTextDirection dir = DIRECTION_LTR,
			SWTextMarkup markup = FMT_UNKNOWN, const char *ilang = 0,
			const char *versification = "KJV");

	virtual ~zText();
	virtual SWBuf &getRawEntryBuf() const;

	virtual void increment(int steps = 1);
	virtual void decrement(int steps = 1) { increment(-steps); }

	// write interface ----------------------------
	virtual bool isWritable() const;
	static char createModule(const char *path, int blockBound, const char *v11n = "KJV") {
		return zVerse::createModule(path, blockBound, v11n);
	}

	virtual void setEntry(const char *inbuf, long len = -1);	// Modify current module entry
	virtual void linkEntry(const SWKey *linkKey);	// Link current module entry to other module entry
	virtual void deleteEntry();	// Delete current module entry
	// end write interface ------------------------

	virtual void rawZFilter(SWBuf &buf, char direction = 0) const { rawFilter(buf, (SWKey *)(long)direction); }// hack, use key as direction for enciphering

	// swcacher interface ----------------------
	virtual void flush() { flushCache(); }
	// end swcacher interface ----------------------

	virtual bool isLinked(const SWKey *k1, const SWKey *k2) const;
	virtual bool hasEntry(const SWKey *k) const;
	
	SWMODULE_OPERATORS
};

SWORD_NAMESPACE_END
#endif