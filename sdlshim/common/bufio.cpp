
#include <stdlib.h>
#include <string.h>
#include "DBuffer.h"

#include "bufio.fdh"

uint8_t read_U8(const uint8_t **data, const uint8_t *data_end)
{
	if (*data > data_end)
	{
		staterr("read_U8: read past end of buffer: *data > data_end");
		return 0xfe;
	}
	
	return *(*data)++;
}

uint16_t read_U16(const uint8_t **data, const uint8_t *data_end)
{
	const uint8_t *ptr = *data;
	
	if ((ptr + 1) > data_end)
	{
		staterr("read_U16: read past end of buffer: *data + 1 > data_end");
		return 0xfefe;
	}
	
	*data = (ptr + 2);
	return *((uint16_t *)ptr);
}

uint32_t read_U32(const uint8_t **data, const uint8_t *data_end)
{
	const uint8_t *ptr = *data;
	
	if ((ptr + 3) > data_end)
	{
		staterr("read_U32: read past end of buffer: *data + 3 > data_end");
		return 0xfefefefe;
	}
	
	*data = (ptr + 4);
	return *((uint32_t *)ptr);
}

uint64_t read_U64(const uint8_t **data, const uint8_t *data_end)
{
	const uint8_t *ptr = *data;
	
	if ((ptr + 7) > data_end)
	{
		staterr("read_U32: read past end of buffer: *data + 7 > data_end");
		return 0xfefefefe;
	}
	
	*data = (ptr + 8);
	return *((uint64_t *)ptr);
}

float read_F32(const uint8_t **data, const uint8_t *data_end)
{
	const uint8_t *ptr = *data;
	
	if ((ptr + 3) > data_end)
	{
		staterr("read_F32: read past end of buffer: *data + 3 > data_end");
		return 0;
	}
	
	*data = (ptr + 4);
	return *((float *)ptr);
}

double read_F64(const uint8_t **data, const uint8_t *data_end)
{
	const uint8_t *ptr = *data;
	
	if ((ptr + 7) > data_end)
	{
		staterr("read_F64: read past end of buffer: *data + 7 > data_end");
		return 0;
	}
	
	*data = (ptr + 8);
	return *((double *)ptr);
}

/*
void c------------------------------() {}
*/

void write_U8(DBuffer *buffer, uint8_t data)
{
	buffer->AppendData((uint8_t *)&data, 1);
}

void write_U16(DBuffer *buffer, uint16_t data)
{
	buffer->AppendData((uint8_t *)&data, 2);
}

void write_U32(DBuffer *buffer, uint32_t data)
{
	buffer->AppendData((uint8_t *)&data, 4);
}

void write_U64(DBuffer *buffer, uint64_t data)
{
	buffer->AppendData((uint8_t *)&data, 8);
}

void write_F32(DBuffer *buffer, float data)
{
	buffer->AppendData((uint8_t *)&data, 4);
}

void write_F64(DBuffer *buffer, double data)
{
	buffer->AppendData((uint8_t *)&data, 8);
}

/*
void c------------------------------() {}
*/

uint32_t read_U24(const uint8_t **data, const uint8_t *data_end)
{
	uint32_t a, b;
	
	a = read_U16(data, data_end);
	b = read_U8(data, data_end);
	
	return a | (b << 16);
}

void write_U24(DBuffer *buffer, uint32_t data)
{
	write_U16(buffer, data);
	write_U8(buffer, data >> 16);
}

/*
void c------------------------------() {}
*/

char read_char(const char **data, const char *data_end)
{
	return (char)read_U8((const uint8_t **)data, (const uint8_t *)data_end);
}

char read_nonblank_char(const char **data, const char *data_end)
{
char ch;

	for(;;)
	{
		if (*data > data_end)
		{
			staterr("read_nonblank_char: read past end of buffer: *data > data_end");
			return 254;
		}
		
		ch = read_char(data, data_end);
		if (ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r') break;
	}
	
	return ch;
}

/*
void c------------------------------() {}
*/

void read_Variable(DBuffer *out, const uint8_t **data, const uint8_t *data_end)
{
	uint8_t len = read_U8(data, data_end);
	const uint8_t *ptr = *data;
	
	if ((ptr + (len - 1)) > data_end)
	{
		staterr("read_Variable: read past end of buffer: *ptr+len > data_end");
		return;
	}
	
	out->SetTo(ptr, len);
	*data = (ptr + len);
}

void write_Variable(DBuffer *out, DBuffer *in)
{
	write_Variable(out, in->Data(), in->Length());
}

void write_Variable(DBuffer *out, const uint8_t *data, int len)
{
	if (len > 255)
	{
		staterr("write_Variable: input length > 255");
		len = 255;
	}
	
	out->AppendChar(len);
	out->AppendData(data, len);
}


void read_Variable16(DBuffer *out, const uint8_t **data, const uint8_t *data_end)
{
	uint16_t len = read_U16(data, data_end);
	const uint8_t *ptr = *data;
	
	if ((ptr + (len - 1)) > data_end)
	{
		staterr("read_Variable16: read past end of buffer: *ptr+len > data_end");
		return;
	}
	
	out->SetTo(ptr, len);
	*data = (ptr + len);
}

void write_Variable16(DBuffer *out, DBuffer *in)
{
	int len = in->Length();
	if (len > 65535)
	{
		staterr("write_Variable16: input length > 65535");
		len = 65535;
	}
	
	out->Append16(len);
	out->AppendData(in->Data(), len);
}


