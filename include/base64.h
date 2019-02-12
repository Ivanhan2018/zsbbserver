#ifndef __BASE64_H__
#define __BASE64_H__

#if defined(c_plusplus) || defined(__cplusplus)
extern "C" {
#endif

void base64_encode(char *, int, char *);
void base64_decode(char *, int, char *);

#if defined(c_plusplus) || defined(__cplusplus)
}
#endif /*c_plusplus __cplusplus*/

#endif // __BASE64_H__
