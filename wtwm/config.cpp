// This source was largely taken from the sxhkd repo, created by baskerville on github
// https://github.com/baskerville/sxhkd/
/* Copyright (c) 2013, Bastien Dejean
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "stdafx.h"
#include "config.h"
#include <iostream>


char *leftmost_graph(char *s)
{
	size_t len = strlen(s);
	unsigned int i = 0;
	while (i < len && !isgraph(s[i]))
		i++;
	if (i < len)
		return (s + i);
	else
		return NULL;
}

char *rightmost_graph(char *s)
{
	size_t i = strlen(s) - 1;
	while (i >= 0 && !isgraph(s[i]))
		i--;
	if (i >= 0)
		return (s + i);
	else
		return NULL;
}

std::vector<std::pair<std::string, std::string>> load_config(const std::string config_path)
{
	std::cout << "load configuration " << config_path << std::endl;
	FILE *cfg;
	errno_t err = fopen_s(&cfg, config_path.c_str(), "r");
	std::vector<std::pair<std::string, std::string>> bindingPairs;
	if (cfg == NULL) {
		std::cout << "Can't open configuration file: '" << config_path << std::endl;
		return bindingPairs;
	}

	char buf[3 * MAXLEN];
	char chain[MAXLEN] = {0};
	char command[2 * MAXLEN] = {0};
	int offset = 0;
	char first;

	while (fgets(buf, sizeof(buf), cfg) != NULL) {
		first = buf[0];
		if (strlen(buf) < 2 || first == START_COMMENT) {
			continue;
		} else {
			char *start = leftmost_graph(buf);
			if (start == NULL)
				continue;
			char *end = rightmost_graph(buf);
			*(end + 1) = '\0';

			if (isgraph(first))
				snprintf(chain + offset, sizeof(chain) - offset, "%s", start);
			else
				snprintf(command + offset, sizeof(command) - offset, "%s", start);

			if (*end == PARTIAL_LINE) {
				offset += end - start;
				continue;
			} else {
				offset = 0;
			}

			if (isspace(first) && strlen(chain) > 0 && strlen(command) > 0) {
				process_hotkey(bindingPairs, chain, command);
				chain[0] = '\0';
				command[0] = '\0';
			}
		}
	}

	fclose(cfg);
	return bindingPairs;
}

void process_hotkey(std::vector<std::pair<std::string, std::string>>& bindingPairs, char *hotkey_string, char *command_string)
{
	char hotkey[MAXLEN] = {0};
	char command[2 * MAXLEN] = {0};
	char last_hotkey[MAXLEN] = {0};
	unsigned char num_same = 0;
	Chunk *hk_chunks = extract_chunks(hotkey_string);
	Chunk *cm_chunks = extract_chunks(command_string);

#define CHECKCHUNK(s, c) \
	if (c->next == NULL && !c->sequence) { \
		snprintf(s, sizeof(s), "%s", c->text); \
		destroy_chunks(c); \
		c = NULL; \
	}
	CHECKCHUNK(hotkey, hk_chunks)
	CHECKCHUNK(command, cm_chunks)
#undef CHECKCHUNK

	render_next(hk_chunks, hotkey);
	render_next(cm_chunks, command);

	while ((hk_chunks == NULL || hotkey[0] != '\0') && (cm_chunks == NULL || command[0] != '\0')) {

		bindingPairs.push_back({ hotkey, command });
		// TODO: Fix infinite loop here with single commands like "ctrl+1"
		render_next(hk_chunks, hotkey);
		render_next(cm_chunks, command);
	}

	if (hk_chunks != NULL)
		destroy_chunks(hk_chunks);
	if (cm_chunks != NULL)
		destroy_chunks(cm_chunks);
}

char *get_token(char *dst, char *ign, char *src, char *sep)
{
	size_t len = strlen(src);
	unsigned int i = 0, j = 0, k = 0;
	bool inhibit = false;
	bool found = false;
	while (i < len && !found) {
		if (inhibit) {
			dst[j++] = src[i];
			inhibit = false;
		} else if (src[i] == MAGIC_INHIBIT) {
			inhibit = true;
			if (src[i+1] != MAGIC_INHIBIT && strchr(sep, src[i+1]) == NULL)
				dst[j++] = src[i];
		} else if (strchr(sep, src[i]) != NULL) {
			if (j > 0)
				found = true;
			do {
				if (ign != NULL)
					ign[k++] = src[i];
				i++;
			} while (i < len && strchr(sep, src[i]) != NULL);
			i--;
		} else {
			dst[j++] = src[i];
		}
		i++;
	}
	dst[j] = '\0';
	if (ign != NULL)
		ign[k] = '\0';
	return src + i;
}

Chunk *extract_chunks(char *s)
{
	size_t len = strlen(s);
	unsigned int i = 0, j = 0;
	bool inhibit = false;
	int num_seq = 0;
	Chunk *c = make_chunk();
	Chunk *head = c;
	while (i < len) {
		if (inhibit) {
			c->text[j++] = s[i];
			inhibit = false;
		} else if (s[i] == MAGIC_INHIBIT) {
			inhibit = true;
			if ((s[i+1] != MAGIC_INHIBIT || c->sequence)
					&& s[i+1] != SEQ_BEGIN
					&& s[i+1] != SEQ_END)
				c->text[j++] = s[i];
		} else if (s[i] == SEQ_BEGIN) {
			if (j > 0) {
				c->text[j] = '\0';
				j = 0;
				Chunk *next = make_chunk();
				c->next = next;
				c = next;
			}
			c->sequence = true;
		} else if (s[i] == SEQ_END) {
			if (c->sequence)
				num_seq++;
			if (j > 0) {
				c->text[j] = '\0';
				j = 0;
				Chunk *next = make_chunk();
				c->next = next;
				c = next;
			}
			c->sequence = false;
		} else {
			c->text[j++] = s[i];
		}
		i++;
	}
	c->text[j] = '\0';
	return head;
}

Chunk *make_chunk(void)
{
	Chunk* c = new Chunk();
	c->sequence = false;
	c->advance = NULL;
	c->next = NULL;
	c->range_cur = 1;
	c->range_max = 0;
	return c;
}

void destroy_chunks(Chunk *chunk)
{
	Chunk *c = chunk;
	while (c != NULL) {
		Chunk *next = c->next;
		delete c;
		c = next;
	}
}

void render_next(Chunk *chunks, char *dest)
{
	if (chunks == NULL) {
		return;
	}
	size_t i = 0;
	bool incr = false;
	for (Chunk *c = chunks; c != NULL; c = c->next) {
		if (c->sequence) {
			if (!incr) {
				if (c->range_cur < c->range_max) {
					c->range_cur++;
					incr = true;
				}
				else {
					c->range_cur = 1, c->range_max = 0;
				}
			}
			if (c->advance == NULL) {
				incr = true;
				c->advance = get_token(c->item, NULL, c->text, SEQ_SEP);
			}
			else if (!incr && c->range_cur > c->range_max) {
				if (c->advance[0] == '\0') {
					c->advance = get_token(c->item, NULL, c->text, SEQ_SEP);
				}
				else {
					c->advance = get_token(c->item, NULL, c->advance, SEQ_SEP);
					incr = true;
				}
			}
			if (c->range_cur > c->range_max && strlen(c->item) == 3)
				sscanf_s(c->item, "%c-%c", &c->range_cur, &c->range_max);
			if (c->range_cur <= c->range_max) {
				dest[i++] = c->range_cur;
			}
			else {
				if (c->item[0] == SEQ_NONE && c->item[1] == '\0')
					continue;
				strcpy(dest + i, c->item);
				i += strlen(c->item);
			}
		}
		else {
			strcpy(dest + i, c->text);
			i += strlen(c->text);
		}
	}
	dest[i] = '\0';
	if (!incr)
		dest[0] = '\0';
}
