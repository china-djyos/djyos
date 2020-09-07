//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合以下三条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。
// 3. 本软件作为都江堰操作系统的组成部分，未获事前取得的书面许可，不允许移植到非
//    都江堰操作系统环境下运行。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------

//所属模块: GDD
//作者:  LiuWei.
//版本：V1.0.0
//文件描述: gif
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2014-11-10
//   作者:  LiuWei.
//   新版本号：V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------

#include    "gdd_private.h"


/*********************************************************************
*
*       Defines
*
**********************************************************************
*/


#define GIF_INTRO_TERMINATOR ';'
#define GIF_INTRO_EXTENSION  '!'
#define GIF_INTRO_IMAGE      ','

#define GIF_COMMENT     0xFE
#define GIF_APPLICATION 0xFF
#define GIF_PLAINTEXT   0x01
#define GIF_GRAPHICCTL  0xF9

#define MAX_NUM_LWZ_BITS 12

/*********************************************************************
*
*       Types
*
**********************************************************************
*/

typedef struct
{
  s32 XPos;
  s32 YPos;
  s32 XSize;
  s32 YSize;
  s32 Flags;
  s32 NumColors;
}IMAGE_DESCRIPTOR;

typedef struct
{

  u8    aBuffer[258];                     /* Input buffer for data block */
  u16   aCode  [(1 << MAX_NUM_LWZ_BITS)]; /* This array stores the LZW codes for the compressed strings */
  u8    aPrefix[(1 << MAX_NUM_LWZ_BITS)]; /* Prefix character of the LZW code. */
  u8    aDecompBuffer[2048];              /* Decompression buffer. The higher the compression, the more bytes are needed in the buffer. */
  u8    *sp;                              /* Pointer into the decompression buffer */
  s32   CurBit;
  s32   LastBit;
  s32   GetDone;
  s32   LastByte;
  s32   ReturnClear;
  s32   CodeSize;
  s32   SetCodeSize;
  s32   MaxCode;
  s32   MaxCodeSize;
  s32   ClearCode;
  s32   EndCode;
  s32   FirstCode;
  s32   OldCode;

} LZW_CONTEXT;

struct  GIF_DECODE
{
  u8 *pCurData;
  u8 *pSrcData;
  s32 NumBytes;
  s32 RemBytes;
  s32 FrameNum;
  s32 NumColors;
  s32 xSize,ySize;
  u32 ColorTable[256];
  u8  Buffer[256];
  IMAGE_DESCRIPTOR ImgDesc;
  LZW_CONTEXT LZW;

};

typedef struct    __GIF_IMAGE_INFO
{
  s32 xPos;
  s32 yPos;
  s32 xSize;
  s32 ySize;
  s32 Delay;
} GIF_IMAGE_INFO;


/*********************************************************************
*
*       Static const
*
**********************************************************************
*/
static const s32 _aMaskTbl[16] = {
  0x0000, 0x0001, 0x0003, 0x0007,
  0x000f, 0x001f, 0x003f, 0x007f,
  0x00ff, 0x01ff, 0x03ff, 0x07ff,
  0x0fff, 0x1fff, 0x3fff, 0x7fff,
};

static const s32 _aInterlaceOffset[] = {  8, 8, 4, 2 };
static const s32 _aInterlaceYPos[]   = {  0, 4, 2, 1 };


/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

/*********************************************************************
*
*       read_u8
*
* Purpose:
*   Reads one byte from the given pointer if possible and increments the pointer
*/

static u8 read_u8(struct GIF_DECODE *decode)
{
  u8 Value;

  decode->RemBytes -= 1;
  if(decode->RemBytes < 0)
  {
    return 0;
  }

  Value = *(decode->pCurData++);
  return Value;
}




/*********************************************************************
*
*       read_u16
*
* Purpose:
*   Reads a word from the given pointer if possible and increments the pointer
*/
static u16 read_u16(struct GIF_DECODE *decode)
{
  u16 Value;
  u8 Byte0, Byte1;
  ////

  Byte0 = read_u8(decode);
  Byte1 = read_u8(decode);
  Value = (Byte1 << 8) | Byte0;
  return Value;
}


/*********************************************************************
*
*       __GDD_ReadBytes
*
* Purpose:
*   Reads a string from the given pointer if possible and increments the pointer
*/
static void __GDD_ReadBytes(struct GIF_DECODE *decode,u8 * pBuffer, s32 Len)
{
  if (decode->RemBytes < 0)
  {
    return;
  }
  if (decode->RemBytes < Len)
  {
    Len = decode->RemBytes;
    return;
  }
  decode->RemBytes -= Len;
  memcpy(pBuffer, decode->pCurData, Len);
  decode->pCurData += Len;

}

/*********************************************************************
*
*       __GDD_SkipBytes
*
* Purpose:
*   Skips the number of given bytes and increments the pointer
*/
static void __GDD_SkipBytes(struct GIF_DECODE *decode,s32 Len)
{
  if (decode->RemBytes < 0)
  {
    return;
  }

  if (decode->RemBytes < Len)
  {
    Len = decode->RemBytes;
    return;
  }

  decode->RemBytes -= Len;
  decode->pCurData += Len;

}

/*********************************************************************
*
*       _InitLWZ
*
* Purpose:
*   Initializes the given LZW with the input code size
*/
static void __GDD_LZW_Init(struct GIF_DECODE *decode,s32 InputCodeSize)
{
  memset((u8 *)&(decode->LZW), 0, sizeof(LZW_CONTEXT));

  decode->LZW.SetCodeSize  = InputCodeSize;
  decode->LZW.CodeSize     = InputCodeSize + 1;
  decode->LZW.ClearCode    = (1 << InputCodeSize);
  decode->LZW.EndCode      = (1 << InputCodeSize) + 1;
  decode->LZW.MaxCode      = (1 << InputCodeSize) + 2;
  decode->LZW.MaxCodeSize  = (1 << InputCodeSize) << 1;
  decode->LZW.ReturnClear  = 1;
  decode->LZW.LastByte     = 2;
  decode->LZW.sp           = decode->LZW.aDecompBuffer;
}

/*********************************************************************
*
*       __GDD_GetDataBlock
*
* Purpose:
*   Reads a LZW data block. The first byte contains the length of the block,
*   so the maximum length is 256 byte
*
* Return value:
*   Length of the data block
*/
static s32 __GDD_GetDataBlock(struct GIF_DECODE *decode,u8 * pBuffer)
{
  u8 Count;

  Count = read_u8(decode); /* Read the length of the data block */
  if (Count)
  {
    if (pBuffer)
    {
      __GDD_ReadBytes(decode,pBuffer, Count);
    }
    else
    {
      decode->pCurData += Count;
    }
  }
  return((s32)Count);
}

/*********************************************************************
*
*       __GDD_GetNextCode
*
* Purpose:
*   Returns the next LZW code from the LZW stack. One LZW code contains up to 12 bits.
*
* Return value:
*   >= 0 if succeed
*   <  0 if not succeed
*/
static s32 __GDD_GetNextCode(struct GIF_DECODE *decode)
{
  s32 i, j, End;
  u32 Result;
  ////

  if (decode->LZW.ReturnClear)
  {
    /* The first code should be a clear code. */
    decode->LZW.ReturnClear = 0;
    return decode->LZW.ClearCode;
  }
  End = decode->LZW.CurBit + decode->LZW.CodeSize;

  if (End >= decode->LZW.LastBit)
  {
    s32 Count;
    ////

    if (decode->LZW.GetDone)
    {
      return -1; /* Error */
    }
    decode->LZW.aBuffer[0] = decode->LZW.aBuffer[decode->LZW.LastByte - 2];
    decode->LZW.aBuffer[1] = decode->LZW.aBuffer[decode->LZW.LastByte - 1];
    if ((Count = __GDD_GetDataBlock(decode,&decode->LZW.aBuffer[2])) == 0)
    {
      decode->LZW.GetDone = 1;
    }

    if (Count < 0)
    {
      return -1; /* Error */
    }

    decode->LZW.LastByte = 2 + Count;
    decode->LZW.CurBit   = (decode->LZW.CurBit - decode->LZW.LastBit) + 16;
    decode->LZW.LastBit  = (2 + Count) * 8 ;
    End                  = decode->LZW.CurBit + decode->LZW.CodeSize;

  }
  ////

  j = End >> 3;
  i = decode->LZW.CurBit >> 3;
  if (i == j)
  {
    Result = (u32)decode->LZW.aBuffer[i];
  }
  else if (i + 1 == j)
  {
    Result = (u32)decode->LZW.aBuffer[i] | ((u32)decode->LZW.aBuffer[i + 1] << 8);
  }
  else
  {
    Result = (u32)decode->LZW.aBuffer[i] | ((u32)decode->LZW.aBuffer[i + 1] << 8) | ((u32)decode->LZW.aBuffer[i + 2] << 16);
  }

  Result = (Result >> (decode->LZW.CurBit & 0x7)) & _aMaskTbl[decode->LZW.CodeSize];
  decode->LZW.CurBit += decode->LZW.CodeSize;
  return (s32)Result;

}

/*********************************************************************
*
*       __GDD_GetNextByte
*
* Purpose:
*   Reads the next LZW code from the LZW stack and returns the first byte from the LZW code.
*
* Return value:
*   >= 0 if succeed
*   -1   if not succeed
*   -2   if end code has been read
*/
static s32 __GDD_GetNextByte(struct GIF_DECODE *decode)
{
  s32 i, Code, Incode;
  while ((Code = __GDD_GetNextCode(decode)) >= 0)
  {
    if (Code == decode->LZW.ClearCode)
    {
      /* Corrupt GIFs can make this happen */
      if (decode->LZW.ClearCode >= (1 << MAX_NUM_LWZ_BITS))
      {
        return -1; /* Error */
      }
      /* Clear the tables */
      memset((u8 *)decode->LZW.aCode, 0, sizeof(decode->LZW.aCode));
      for (i = 0; i < decode->LZW.ClearCode; ++i)
      {
        decode->LZW.aPrefix[i] = i;
      }
      /* Calculate the 'special codes' in dependence of the initial code size
         and initialize the stack pointer */
      decode->LZW.CodeSize    = decode->LZW.SetCodeSize + 1;
      decode->LZW.MaxCodeSize = decode->LZW.ClearCode << 1;
      decode->LZW.MaxCode     = decode->LZW.ClearCode + 2;
      decode->LZW.sp          = decode->LZW.aDecompBuffer;
      /* Read the first code from the stack after clearing and initializing */
      do
      {
        decode->LZW.FirstCode = __GDD_GetNextCode(decode);
      } while (decode->LZW.FirstCode == decode->LZW.ClearCode);
      decode->LZW.OldCode = decode->LZW.FirstCode;
      return decode->LZW.FirstCode;
    }

    if (Code == decode->LZW.EndCode)
    {
      return -2; /* End code */
    }

    Incode = Code;

    if (Code >= decode->LZW.MaxCode)
    {
      *(decode->LZW.sp)++ = decode->LZW.FirstCode;
      Code = decode->LZW.OldCode;
    }

    while (Code >= decode->LZW.ClearCode)
    {
      *(decode->LZW.sp)++ = decode->LZW.aPrefix[Code];
      if (Code == decode->LZW.aCode[Code])
      {
        return Code;
      }

      if ((decode->LZW.sp - decode->LZW.aDecompBuffer) >= sizeof(decode->LZW.aDecompBuffer))
      {
        return Code;
      }
      Code = decode->LZW.aCode[Code];
    }

    *(decode->LZW.sp)++ = decode->LZW.FirstCode = decode->LZW.aPrefix[Code];
    if ((Code = decode->LZW.MaxCode) < (1 << MAX_NUM_LWZ_BITS))
    {
      decode->LZW.aCode  [Code] = decode->LZW.OldCode;
      decode->LZW.aPrefix[Code] = decode->LZW.FirstCode;
      ++decode->LZW.MaxCode;
      if ((decode->LZW.MaxCode >= decode->LZW.MaxCodeSize) && (decode->LZW.MaxCodeSize < (1 << MAX_NUM_LWZ_BITS)))
      {
        decode->LZW.MaxCodeSize <<= 1;
        ++decode->LZW.CodeSize;
      }
    }

    decode->LZW.OldCode = Incode;
    if (decode->LZW.sp > decode->LZW.aDecompBuffer)
    {
      return *--(decode->LZW.sp);
    }
  }

  return Code;

}

/*********************************************************************
*
*       __GDD_ReadExtension
*
* Purpose:
*   Reads an extension block. One extension block can consist of several data blocks.
*   If an unknown extension block occures, the routine failes.
*/

static s32 __GDD_ReadExtension(struct GIF_DECODE *decode,s32 * pTransIndex, GIF_IMAGE_INFO * pInfo, u8 * pDisposal)
{
  u8 Label;
  ////

  Label = read_u8(decode);
  switch (Label)
  {
      case GIF_PLAINTEXT:
      case GIF_APPLICATION:
      case GIF_COMMENT:

        while (__GDD_GetDataBlock(decode,decode->Buffer) > 0);
        return 0;
        ////

      case GIF_GRAPHICCTL:

        if (__GDD_GetDataBlock(decode,decode->Buffer) != 4)
        { /* Length of a graphic control block must be 4 */
          return 1;
        }
        ////

        if (pInfo)
        {
          pInfo->Delay    = (decode->Buffer[2] << 8) | decode->Buffer[1];
        }
        ////

        if (pDisposal)
        {
          *pDisposal = (decode->Buffer[0] >> 2) & 0x7;
        }
        ////

        if (pTransIndex)
        {
          if ((decode->Buffer[0] & 0x1) != 0)
          {
            *pTransIndex = decode->Buffer[3];
          }
        }
        ////

        if (read_u8(decode) != 0)
        { /* Read block terminator */
          return 1;
        }

        return 0;

  }

  return 1; /* Error */

}

/*********************************************************************
*
*       __GDD_DisplayGifImage
*
* Purpose:
*   This routine draws a GIF image from the current pointer which should point to a
*   valid GIF data block. The size of the desired image is given in the image descriptor.
*
* Return value:
*   TRUE if succeed
*   FALSE if not succeed
*
* Parameters:
*   pDescriptor  - Points to a IMAGE_DESCRIPTOR structure, which contains infos about size, colors and interlacing.
*   x0, y0       - Obvious.
*   Transparency - Color index which should be treated as transparent.
*   Disposal     - Contains the disposal method of the previous image. If Disposal == 2, the transparent pixels
*                  of the image are rendered with the background color.
*/


static bool_t __GDD_DisplayGifImage(HDC hdc,s32 x, s32 y,u32 bk_color,struct GIF_DECODE *hgif, s32 TransIndex, s32 Disposal)
{

  s32 Index, OldIndex, XPos, YPos, YCnt, Pass, Interlace, XEnd;
  s32 Height, Cnt;
  u32 color;
  ////

  Height        = hgif->ImgDesc.YSize;
  XEnd          = hgif->ImgDesc.XSize + x - 1;

  Cnt      = read_u8(hgif);                /* Read the LZW codesize */
  __GDD_LZW_Init(hgif,Cnt);                      /* Initialize the LZW stack with the LZW codesize */
  Interlace = hgif->ImgDesc.Flags & 0x40;  /* Evaluate if image is interlaced */

  for (YCnt = 0, YPos = y, Pass = 0; YCnt < Height; YCnt++)
  {
    Cnt      = 0;
    OldIndex = -1;

    for (XPos = x; XPos <= XEnd; XPos++)
    {

      if (hgif->LZW.sp > hgif->LZW.aDecompBuffer)
      {
        Index = *--(hgif->LZW.sp);
      }
      else
      {
        Index = __GDD_GetNextByte(hgif);
      }
      ////

      if (Index == -2)
      {
        return TRUE; /* End code */
      }

      if ((Index < 0) || (Index >= hgif->ImgDesc.NumColors))
      {
        /* If Index out of legal range stop decompressing */
        return FALSE; /* Error */
      }

      //GDD_SetPixel(hdc,XPos-1,YPos,decode->ColorTable[Index]);

      #if 1
      /* If current index equals old index increment counter */
      if ((Index == OldIndex) && (XPos <= XEnd))
      {
        Cnt++;
      }
      else
      {
        if (Cnt)
        {
          if (OldIndex != TransIndex)
          {
               color =hgif->ColorTable[OldIndex];
               GDD_DrawLineEx(hdc,XPos - Cnt - 1, YPos, XPos-0,YPos,color);
          }
          else if(Disposal == 2)
          {

              GDD_DrawLineEx(hdc,XPos - Cnt - 1, YPos, XPos-0,YPos,bk_color);
          }

          Cnt = 0;

        }
        else
        {
          if (OldIndex >= 0)
          {
            if (OldIndex != TransIndex)
            {
                color   =hgif->ColorTable[OldIndex];
                GDD_SetPixel(hdc,XPos-1,YPos,color);
            }
            else if (Disposal == 2)
            {
                GDD_SetPixel(hdc,XPos-1,YPos,bk_color);
            }

          }
        }
      }
      #endif
      OldIndex = Index;

    }
    ////
    if((OldIndex != TransIndex) || (Disposal == 2))
    {

      if (OldIndex != TransIndex)
      {
        color   =hgif->ColorTable[OldIndex];
        if(Cnt)
        {

          GDD_DrawLineEx(hdc,XPos - Cnt - 1, YPos, XPos-0,YPos,color);
        }
        else
        {
          GDD_SetPixel(hdc,XEnd,YPos,color);
        }
      }
      else
      {
        if(Cnt)
        {
          GDD_DrawLineEx(hdc,XPos - Cnt - 1, YPos, XPos-0,YPos,bk_color);
        }
        else
        {
          GDD_SetPixel(hdc,XEnd,YPos,bk_color);
        }
      }


    }

    /* Adjust YPos if image is interlaced */
    if(Interlace)
    {
      YPos += _aInterlaceOffset[Pass];
      if ((YPos - y) >= Height)
      {
        ++Pass;
        YPos = _aInterlaceYPos[Pass] + y;
      }
    }
    else
    {
      YPos++;
    }

  }

  return TRUE;

}


/*********************************************************************
*
*       __GDD_ReadColorMap
*/
static s32 __GDD_ReadColorMap(struct GIF_DECODE *decode,s32 NumColors)
{
  s32 i,r,g,b;
  ////

  for (i = 0; i < NumColors; i++)
  {
    r = read_u8(decode);
    g = read_u8(decode);
    b = read_u8(decode);

    decode->ColorTable[i] = RGB(r,g,b);
  }


  return 0;
}

/*********************************************************************
*
*       __GDD_GifDecodeInit
*
* Purpose:
*   The routine initializes the static decode structure and checks
*   if the file is a legal GIF file.
*
* Return value:
*   0 on success, 1 on error
*/

static s32 __GDD_GifDecodeInit(struct GIF_DECODE *decode)
{
      u8 acVersion[7] = {0};
      ////

     decode->pCurData    = decode->pSrcData;
     decode->RemBytes     = decode->NumBytes;

      /* Check if the file is a legal GIF file by checking the 6 byte file header */
     __GDD_ReadBytes(decode,acVersion, 6); if (!decode->RemBytes) { return 0; }
     if ( (acVersion[0] != 'G') ||
       (acVersion[1] != 'I') ||
       (acVersion[2] != 'F') ||
       (acVersion[3] != '8') ||
      ((acVersion[4] != '7') && (acVersion[4] != '9')) ||
       (acVersion[5] != 'a'))
      {
        return 1;
      }
      return 0;
}

/*********************************************************************
*
*       __GDD_GetImageDimension
*
* Purpose:
*   Reads the image dimension from the logical screen descriptor
*
* Return value:
*   0 on success, 1 on error
*/

static s32 __GDD_GetImageDimension(struct GIF_DECODE *decode)
{
  s32 XSize, YSize;
  /* Read image size */
  XSize = read_u16(decode);
  YSize = read_u16(decode);
  if ((XSize > 2000) || (YSize > 2000))
  {
    return 1; /* Error if image is too large */
  }
  decode->xSize    =XSize;
  decode->ySize    =YSize;
  return 0;

}


/*********************************************************************
*
*       __GDD_GetGlobalColorTable
*
* Purpose:
*   Reads the global color table if there is one. Returns the number of
*   available colors over the pointer pNumColors (can be NULL).
*
* Return value:
*   0 on success, 1 on error
*/
static s32 __GDD_GetGlobalColorTable(struct GIF_DECODE *decode)
{

  u8 Flags;
  s32 NumColors;
  /* Read flags from logical screen descriptor */
  Flags = read_u8 (decode);
  read_u8 (decode);
  read_u8 (decode);
  if (decode->RemBytes < 0)
  {
    return 1; /* Error */
  }
  NumColors = 2 << (Flags & 0x7);
  if (Flags & 0x80)
  {
    /* Read global color table */
    if (__GDD_ReadColorMap(decode,NumColors))
    {
      return 1; /* Error */
    }
  }

  decode->NumColors            =NumColors;
  decode->ImgDesc.NumColors    =NumColors;

  return 0;
}

/*********************************************************************
*
*       __GDD_InitSizeAndColorTable
*/

static s32 __GDD_InitSizeAndColorTable(struct GIF_DECODE *decode)
{

  /* Initialize decoding */
  if (__GDD_GifDecodeInit(decode))
  {
    return 1; /* Error */
  }

  /* Get image size */
  if (__GDD_GetImageDimension(decode))
  {
    return 1; /* Error */
  }

  /* Get global color table (if available) */
  if (__GDD_GetGlobalColorTable(decode))
  {
    return 1; /* Error */
  }
  return 0;
}


static bool_t __GDD_GifInit(struct GIF_DECODE *decode)
{
  u8 Flags, Introducer;
  s32 ImageCnt;
  ////

  /* Initialize decoding and get size and global color table */
  if (__GDD_InitSizeAndColorTable(decode))
  {
    return FALSE; /* Error */
  }


  ImageCnt = 0;
  /* Iterate over the blocks */
  do
  {
    Introducer = read_u8(decode);
    switch (Introducer)
    {
        case GIF_INTRO_IMAGE:
          __GDD_SkipBytes(decode,8);                /* Skip the first 8 bytes of the image descriptor */
          Flags = read_u8(decode);            /* Only 'Flags' are intresting */
          if (Flags & 0x80)
          {
            __GDD_SkipBytes(decode,decode->NumColors * 3);  /* Skip local color table */
          }
          __GDD_SkipBytes(decode,1);                /* Skip codesize */
          while (__GDD_GetDataBlock(decode,0) > 0); /* Skip data blocks */
          ImageCnt++;
          break;
          ////

        case GIF_INTRO_TERMINATOR:
          break;
          ////

        case GIF_INTRO_EXTENSION:
          if (__GDD_ReadExtension(decode,NULL, NULL, NULL))
          { /* Skip image extension */
            return FALSE;
          }
          break;
          ////

        default:
          return FALSE;
    }


  } while (Introducer != GIF_INTRO_TERMINATOR); /* We do not support more than one image, so stop when the first terminator has been read */

  decode->FrameNum = ImageCnt;

  return TRUE;

}

/*============================================================================*/
//获得GIF图像信息
static    bool_t __GDD_GetGifImageInfo(struct GIF_DECODE *decode,s32 Index,GIF_IMAGE_INFO * pInfo)
{
  u8 Flags, Introducer;
  s32 NumColors=0, ImageCnt=0;
  ////


  if(decode==NULL)    return FALSE;

  /* Initialize decoding and get size and global color table */
  if (__GDD_InitSizeAndColorTable(decode))
  {
    return FALSE; /* Error */
  }
  ImageCnt = 0;
  /* Iterate over the blocks */
  do {
    Introducer = read_u8(decode);
    switch (Introducer)
    {
        case GIF_INTRO_IMAGE:
          if (Index == ImageCnt)
          {
            pInfo->xPos  = read_u16(decode);
            pInfo->xPos  = read_u16(decode);
            pInfo->xSize = read_u16(decode);
            pInfo->ySize = read_u16(decode);
            return TRUE;
          }

          __GDD_SkipBytes(decode,8);                /* Skip the first 8 bytes of the image descriptor */
          Flags = read_u8(decode);            /* Only 'Flags' are intresting */
          if (Flags & 0x80)
          {
            __GDD_SkipBytes(decode,NumColors * 3);  /* Skip local color table */
          }

          __GDD_SkipBytes(decode,1);                /* Skip codesize */
          while (__GDD_GetDataBlock(decode,0) > 0); /* Skip data blocks */
          ImageCnt++;
          break;

        case GIF_INTRO_TERMINATOR:
          break;

        case GIF_INTRO_EXTENSION:
          if (__GDD_ReadExtension(decode,NULL, (Index == ImageCnt) ? pInfo : NULL, NULL))
          {
            return FALSE;
          }
          break;

        default:
          return FALSE;
    }

  } while (Introducer != GIF_INTRO_TERMINATOR); /* We do not support more than one image, so stop when the first terminator has been read */

  return TRUE;
}

//---- GDD_OpenGIF -----------------------------------------------------------------
//描述: 此函数创建一个新的GIF解码上下文．
//参数：dat: GIF图像数据源
//      size:　GIF图像数据总字节数
//返回：成功则返回GIF解码上下文，失败返回NULL
//------------------------------------------------------------------------------
struct GIF_DECODE*    GDD_OpenGIF(const void *dat,u32 size)
{
    struct GIF_DECODE *hgif;
    u8 *p;
    ////

    p =(u8*)dat;

    if(p[0]!='G')    return NULL;
    if(p[1]!='I')    return NULL;
    if(p[2]!='F')    return NULL;

    hgif=malloc(sizeof(struct GIF_DECODE));
    if(hgif==NULL)
    {
        return NULL;
    }
    memset(hgif, 0, sizeof(struct GIF_DECODE));
    hgif->pSrcData    =(u8*)dat;
    hgif->NumBytes    =size;
    if(!__GDD_GifInit(hgif))
    {
        free(hgif);
        hgif=NULL;
    }

    return hgif;

}

//---- GDD_GetGIF_Width -------------------------------------------------------------
//描述: 获取GIF图像宽度(像素单位)．
//参数：hgif: GIF解码上下文
//返回：成功则返回GIF图像宽度，失败返回0
//------------------------------------------------------------------------------
u32    GDD_GetGIF_Width(struct GIF_DECODE *hgif)
{

    if(hgif!=NULL)
    {
        return hgif->xSize;
    }
    return 0;

}

//---- GDD_GetGIF_Height ------------------------------------------------------------
//描述: 获取GIF图像高度(像素单位)．
//参数：hgif: GIF解码上下文
//返回：成功则返回GIF图像高度，失败返回0
//------------------------------------------------------------------------------
u32    GDD_GetGIF_Height(struct GIF_DECODE *hgif)
{
    if(hgif!=NULL)
    {
        return hgif->ySize;
    }
    return 0;
}

//---- GDD_GetGIF_FrameCount --------------------------------------------------------
//描述: 获取GIF图像总帧数．
//参数：hgif: GIF解码上下文
//返回：成功则返回GIF图像总帧数，失败返回0
//------------------------------------------------------------------------------
u32    GDD_GetGIF_FrameCount(struct GIF_DECODE *hgif)
{
    if(hgif!=NULL)
    {
        return hgif->FrameNum;
    }
    return 0;
}

//---- GDD_GetGIF_FrameDelay --------------------------------------------------------
//描述: 获取GIF图像指定帧的延时时间(毫秒单位)．
//参数：hgif: GIF解码上下文
//      frame_idx: 帧索引值
//返回：成功则返回指定帧的延时时间，失败返回-1
//------------------------------------------------------------------------------
s32    GDD_GetGIF_FrameDelay(struct GIF_DECODE *hgif,u32 frame_idx)
{
    GIF_IMAGE_INFO Info;

    if(hgif!=NULL)
    {
        if(__GDD_GetGifImageInfo(hgif,frame_idx,&Info))
        {
            return Info.Delay;
        }
    }
    return -1;
}

//---- GDD_DrawGIF_Frame ------------------------------------------------------------
//描述: 绘制GIF指定帧
//参数：
//      hdc:  绘图上下文
//      x,y:  绘制的坐标位置(像素值)
//      hgif: GIF解码上下文
//      bk_color: GIF图像背景色
//      frame_idx: 帧索引值
//返回：成功返回TRUE，失败返回FALSE
//------------------------------------------------------------------------------
bool_t    GDD_DrawGIF_Frame(HDC hdc,s32 x,s32 y,struct GIF_DECODE *hgif,u32 bk_color,u32 frame_idx)
{
  s32 TransIndex, ImageCnt,Index;
  GIF_IMAGE_INFO Info;
  u8 Introducer;
  u8 Disposal;
  ////

  ImageCnt   =  0;
  TransIndex = -1;
  Index      = frame_idx;
  ////

  __GDD_InitSizeAndColorTable(hgif);

  do
  {

        Introducer = read_u8(hgif);
        switch (Introducer)
        {
            case GIF_INTRO_IMAGE:
              /* Read image descriptor */
                hgif->ImgDesc.XPos  = read_u16(hgif);
                hgif->ImgDesc.YPos  = read_u16(hgif);
                hgif->ImgDesc.XSize = read_u16(hgif);
                hgif->ImgDesc.YSize = read_u16(hgif);
                hgif->ImgDesc.Flags = read_u8 (hgif);

              if(hgif->RemBytes < 0)
              {
                return FALSE; /* Error */
              }

              if(hgif->ImgDesc.Flags & 0x80)
              {
                /* Read local color table */
                if (__GDD_ReadColorMap(hgif,hgif->ImgDesc.NumColors))
                {
                  return FALSE; /* Error */
                }
              }

              if(ImageCnt == Index - 1)
              {
                Info.xPos  = hgif->ImgDesc.XPos;
                Info.yPos  = hgif->ImgDesc.YPos;
                Info.xSize = hgif->ImgDesc.XSize;
                Info.ySize = hgif->ImgDesc.YSize;
              }

              if(ImageCnt == Index)
              {
                if(Disposal == 2)
                {
                    RECT rc;
                    GDD_SetRect(&rc,x+Info.xPos,y+Info.yPos,Info.xSize,Info.ySize);
                    GDD_FillRectEx(hdc,&rc,bk_color);
                }

                if(!__GDD_DisplayGifImage(hdc,x + hgif->ImgDesc.XPos, y + hgif->ImgDesc.YPos,bk_color,hgif, TransIndex, Disposal))
                {
                  return FALSE;
                }

                if(read_u8(hgif) != 0)
                {
                  return FALSE;
                }
                return TRUE;

              }
              else
              {
                read_u8(hgif);                    /* Skip codesize */
                while (__GDD_GetDataBlock(hgif,0) > 0); /* Skip data blocks */
              }

              ImageCnt++;
              break;
              ////////

            case GIF_INTRO_TERMINATOR:
              break;
              ////////

            case GIF_INTRO_EXTENSION:
              /* Read image extension */
              if (__GDD_ReadExtension(hgif,&TransIndex, (Index == ImageCnt) ? &Info : NULL, (Index == ImageCnt) ? &Disposal : NULL))
              {
                return FALSE;
              }
              break;
              ////////

            default:
              return FALSE;

        }


  } while(Introducer != GIF_INTRO_TERMINATOR);

  return TRUE;

}

//---- GDD_CloseGIF ---------------------------------------------------------------
//描述: 释放一个GIF上下文
//参数：
//      hgif: GIF解码上下文
//返回：无
//-----------------------------------------------------------------------------
void    GDD_CloseGIF(struct GIF_DECODE *hgif)
{
    if(hgif)
    {
        free((void*)hgif);
    }
}

/*============================================================================*/
