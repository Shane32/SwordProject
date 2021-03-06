/******************************************************************************
 *
 *  gbfxhtml.cpp -	GBF to classed XHTML
 *
 * $Id: gbfxhtml.cpp 3726 2020-04-26 17:53:51Z scribe $
 *
 * Copyright 2011-2013 CrossWire Bible Society (http://www.crosswire.org)
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

 
#include <stdlib.h>
#include <gbfxhtml.h>
#include <swmodule.h>
#include <utilxml.h>
#include <versekey.h>
#include <ctype.h>
#include <url.h>

SWORD_NAMESPACE_START

const char *GBFXHTML::getHeader() const {
	return "\
		.wordsOfJesus {\
			color: red;\
		}\
	";
}

GBFXHTML::MyUserData::MyUserData(const SWModule *module, const SWKey *key) : BasicFilterUserData(module, key) {
	if (module) {
		version = module->getName(); 
	}	
}

GBFXHTML::GBFXHTML() {
	setTokenStart("<");
	setTokenEnd(">");
	
	setTokenCaseSensitive(true);

	//addTokenSubstitute("Rf", ")</small></font>");
	addTokenSubstitute("FA", "<font color=\"#800000\">"); // for ASV footnotes to mark text
	addTokenSubstitute("Rx", "</a>");
	addTokenSubstitute("FI", "<i>"); // italics begin
	addTokenSubstitute("Fi", "</i>");
	addTokenSubstitute("FB", "<b>"); // bold begin
	addTokenSubstitute("Fb", "</b>");
	addTokenSubstitute("FR", "<span class=\"wordsOfJesus\">"); // words of Jesus begin
	addTokenSubstitute("Fr", "</span>");
	addTokenSubstitute("FU", "<u>"); // underline begin
	addTokenSubstitute("Fu", "</u>");
	addTokenSubstitute("FO", "<cite>"); //  Old Testament quote begin
	addTokenSubstitute("Fo", "</cite>");
	addTokenSubstitute("FS", "<sup>"); // Superscript begin// Subscript begin
	addTokenSubstitute("Fs", "</sup>");
	addTokenSubstitute("FV", "<sub>"); // Subscript begin
	addTokenSubstitute("Fv", "</sub>");
	addTokenSubstitute("TT", "<big>"); // Book title begin
	addTokenSubstitute("Tt", "</big>");
	addTokenSubstitute("PP", "<cite>"); //  poetry  begin
	addTokenSubstitute("Pp", "</cite>");
	addTokenSubstitute("Fn", "</font>"); //  font  end
	addTokenSubstitute("CL", "<br />"); //  new line
	addTokenSubstitute("CM", "<!P><br />"); //  paragraph <!P> is a non showing comment that can be changed in the front end to <P> if desired
	addTokenSubstitute("CG", ""); //  ???
	addTokenSubstitute("CT", ""); // ???
	addTokenSubstitute("JR", "<div align=\"right\">"); // right align begin
	addTokenSubstitute("JC", "<div align=\"center\">"); // center align begin
	addTokenSubstitute("JL", "</div>"); // align end
	
	renderNoteNumbers = false;
}


bool GBFXHTML::handleToken(SWBuf &buf, const char *token, BasicFilterUserData *userData) {
	const char *tok;
	MyUserData *u = (MyUserData *)userData;

	if (!substituteToken(buf, token)) {
		XMLTag tag(token);
		
		if (!strncmp(token, "WG", 2)) { // strong's numbers
			//buf += " <small><em>&lt;<a href=\"type=Strongs value=";
			buf += " <small><em class=\"strongs\">&lt;<a href=\"passagestudy.jsp?action=showStrongs&type=Greek&value=";
			for (tok = token+2; *tok; tok++)
				//if(token[i] != '\"')
					buf += *tok;
			buf += "\" class=\"strongs\">";
			for (tok = token + 2; *tok; tok++)
				//if(token[i] != '\"')
					buf += *tok;
			buf += "</a>&gt;</em></small>";
		}
		else if (!strncmp(token, "WH", 2)) { // strong's numbers
			//buf += " <small><em>&lt;<a href=\"type=Strongs value=";
			buf += " <small><em class=\"strongs\">&lt;<a href=\"passagestudy.jsp?action=showStrongs&type=Hebrew&value=";
			for (tok = token+2; *tok; tok++)
				//if(token[i] != '\"')
					buf += *tok;
			buf += "\" class=\"strongs\">";
			for (tok = token + 2; *tok; tok++)
				//if(token[i] != '\"')
					buf += *tok;
			buf += "</a>&gt;</em></small>";
		}
		else if (!strncmp(token, "WTG", 3)) { // strong's numbers tense
			//buf += " <small><em>(<a href=\"type=Strongs value=";
			buf += " <small><em class=\"strongs\">(<a href=\"passagestudy.jsp?action=showStrongs&type=Greek&value=";
			for (tok = token + 3; *tok; tok++)
				if(*tok != '\"')
					buf += *tok;
			buf += "\" class=\"strongs\">";
			for (tok = token + 3; *tok; tok++)
				if(*tok != '\"')
					buf += *tok;
			buf += "</a>)</em></small>";
		}
		else if (!strncmp(token, "WTH", 3)) { // strong's numbers tense
			//buf += " <small><em>(<a href=\"type=Strongs value=";
			buf += " <small><em class=\"strongs\">(<a href=\"passagestudy.jsp?action=showStrongs&type=Hebrew&value=";
			for (tok = token + 3; *tok; tok++)
				if(*tok != '\"')
					buf += *tok;
			buf += "\" class=\"strongs\">";
			for (tok = token + 3; *tok; tok++)
				if(*tok != '\"')
					buf += *tok;
			buf += "</a>)</em></small>";
		}

		else if (!strncmp(token, "WT", 2) && strncmp(token, "WTH", 3) && strncmp(token, "WTG", 3)) { // morph tags
			//buf += " <small><em>(<a href=\"type=morph class=none value=";
			buf += " <small><em class=\"morph\">(<a href=\"passagestudy.jsp?action=showMorph&type=Greek&value=";
			
			for (tok = token + 2; *tok; tok++)
				if(*tok != '\"')
					buf += *tok;
			buf += "\" class=\"morph\">";
			for (tok = token + 2; *tok; tok++)				
				if(*tok != '\"') 			
					buf += *tok;		
			buf += "</a>)</em></small>";
		}

		else if (!strcmp(tag.getName(), "RX")) {
			buf += "<a href=\"";
			for (tok = token + 3; *tok; tok++) {
			  if(*tok != '<' && *tok+1 != 'R' && *tok+2 != 'x') {
			    buf += *tok;
			  }
			  else {
			    break;
			  }
			}
			buf += "\">";
		}
		else if (!strcmp(tag.getName(), "RF")) {
			SWBuf type = tag.getAttribute("type");
			SWBuf footnoteNumber = tag.getAttribute("swordFootnote");
			SWBuf noteName = tag.getAttribute("n");
			SWBuf classExtras = "";

			if (type.size()) {
				classExtras.append(" ").append(type);
			}

			if (u->vkey) {
				// leave this special osis type in for crossReference notes types?  Might thml use this some day? Doesn't hurt.
				//char ch = ((tag.getAttribute("type") && ((!strcmp(tag.getAttribute("type"), "crossReference")) || (!strcmp(tag.getAttribute("type"), "x-cross-ref")))) ? 'x':'n');
				buf.appendFormatted("<a class=\"noteMarker%s\" href=\"passagestudy.jsp?action=showNote&type=n&value=%s&module=%s&passage=%s\"><small><sup class=\"n\">*n%s</sup></small></a> ", 
					classExtras.c_str(),
					URL::encode(footnoteNumber.c_str()).c_str(),
					URL::encode(u->version.c_str()).c_str(), 
					URL::encode(u->vkey->getText()).c_str(), 
					(renderNoteNumbers ? URL::encode(noteName.c_str()).c_str(): ""));
			}
			u->suspendTextPassThru = true;
		}
		else if (!strcmp(tag.getName(), "Rf")) {
			u->suspendTextPassThru = false;
		}
/*
		else if (!strncmp(token, "RB", 2)) {
			buf += "<i> ";
			u->hasFootnotePreTag = true;
		}

		else if (!strncmp(token, "Rf", 2)) {
			buf += "&nbsp<a href=\"note=";
			buf += u->lastTextNode.c_str();
			buf += "\">";
			buf += "<small><sup>*n</sup></small></a>&nbsp";
			// let's let text resume to output again
			u->suspendTextPassThru = false;
		}
		
		else if (!strncmp(token, "RF", 2)) {
			if (u->hasFootnotePreTag) {
				u->hasFootnotePreTag = false;
				buf += "</i> ";
			}
			u->suspendTextPassThru = true;
		}
*/
		else if (!strncmp(token, "FN", 2)) {
			buf += "<font face=\"";
			for (tok = token + 2; *tok; tok++)				
				if(*tok != '\"') 			
					buf += *tok;
			buf += "\">";
		}

		else if (!strncmp(token, "CA", 2)) {	// ASCII value
			buf += (char)atoi(&token[2]);
		}
		
		else {
			return false;
		}
	}
	return true;
}

SWORD_NAMESPACE_END
