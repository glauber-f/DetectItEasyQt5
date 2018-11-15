// Copyright (c) 2012-2018 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#include "versionresource.h"

VersionResource::VersionResource(QByteArray *pbaData)
{
    setData(pbaData);
}

void VersionResource::setData(QByteArray *pbaData)
{
    this->pbaData=pbaData;
}

bool VersionResource::isValid()
{
    if(pbaData->size()>=4)
    {
        if(((_VS_VERSION_INFO *)pbaData->data())->wLength<=pbaData->size())
        {
            if(((_VS_VERSION_INFO *)pbaData->data())->wValueLength<((_VS_VERSION_INFO *)pbaData->data())->wLength)
            {
                return true;
            }
        }
    }

    emit appendError("Invalid version resource");
    return false;
}

unsigned int VersionResource::getLength(unsigned int nOffset)
{
    return ((_VS_VERSION_INFO *)(pbaData->data()+nOffset))->wLength;
}

unsigned int VersionResource::getValueLength(unsigned int nOffset)
{
    return ((_VS_VERSION_INFO *)(pbaData->data()+nOffset))->wValueLength;
}

unsigned int VersionResource::getType(unsigned int nOffset)
{
    return ((_VS_VERSION_INFO *)(pbaData->data()+nOffset))->wType;
}
QString VersionResource::getKey(unsigned int nOffset)
{
    return QString::fromUtf16((unsigned short *)(pbaData->data()+nOffset+sizeof(_VS_VERSION_INFO)));
}

QString VersionResource::getValue(unsigned int nOffset)
{
    return QString::fromUtf16((unsigned short *)(pbaData->data()+getNextHeaderOffset(nOffset)));
}

unsigned int VersionResource::getDword(unsigned int nOffset)
{
    return *(unsigned int *)(pbaData->data()+nOffset);
}

QString VersionResource::getFileVersion()
{
    return QString("%1.%2").arg(versionToString(getFixedFileInfo_FileVersionMS())).arg(versionToString(getFixedFileInfo_FileVersionLS()));
}

QString VersionResource::getStringFileInfo(QString sKey)
{
    QString sResult;

    if(isChildrenPresent())
    {

        unsigned int nOffset=getChildrenOffset();
        unsigned int nOffset2=0;
        int nLength =pbaData->size()-nOffset;
        unsigned int nSize=0;
        unsigned int nSize2=0;
        unsigned int nTemp=0;

        while(nLength>0)
        {
            //nSize2=ALIGN_UP(vr.getLength(nOffset),4);
            if(pbaData->size()-(int)nOffset<=0)
            {
                break;
            }

            nSize2=getLength(nOffset);

            if(nSize2==0)
            {
                break;
            }

            if(getKey(nOffset)=="StringFileInfo")
            {
                nOffset=getNextHeaderOffset(nOffset);
                nSize=getLength(nOffset);

                nOffset2=getNextHeaderOffset(nOffset);
                nSize=nSize-(nOffset2-nOffset);
                nOffset=nOffset2;

                while((int)nSize>0)
                {
                    nTemp=ALIGN_UP(getLength(nOffset),4);

                    if(nTemp==0)
                    {
                        break;
                    }

                    if(getKey(nOffset)==sKey)
                    {
                        return getValue(nOffset);
                    }

                    nSize-=nTemp;
                    nOffset+=nTemp;
                }
            }

            nLength-=nSize2;
            nOffset+=nSize2;
        }
    }

    return sResult;
}

QString VersionResource::versionToString(unsigned int nValue)
{
    return QString("%1.%2").arg(__HIWORD(nValue)).arg(__LOWORD(nValue));
}

QString VersionResource::fileFlagsToString(unsigned int nFileFlags)
{
    if(nFileFlags==0)
    {
        return "no";
    }

    QString sResult="";

    if(nFileFlags&_VS_FF_DEBUG)
    {
        if(sResult!="")
        {
            sResult+="|";
        }

        sResult+="VS_FF_DEBUG";
    }

    if(nFileFlags&_VS_FF_INFOINFERRED)
    {
        if(sResult!="")
        {
            sResult+="|";
        }

        sResult+="VS_FF_INFOINFERRED";
    }

    if(nFileFlags&_VS_FF_PATCHED)
    {
        if(sResult!="")
        {
            sResult+="|";
        }

        sResult+="VS_FF_PATCHED";
    }

    if(nFileFlags&_VS_FF_PRERELEASE)
    {
        if(sResult!="")
        {
            sResult+="|";
        }

        sResult+="VS_FF_PRERELEASE";
    }

    if(nFileFlags&_VS_FF_PRIVATEBUILD)
    {
        if(sResult!="")
        {
            sResult+="|";
        }

        sResult+="VS_FF_PRIVATEBUILD";
    }

    if(nFileFlags&_VS_FF_SPECIALBUILD)
    {
        if(sResult!="")
        {
            sResult+="|";
        }

        sResult+="VS_FF_SPECIALBUILD";
    }


    return sResult;
}

QString VersionResource::fileOSToString(unsigned int nFileOS)
{
    if(nFileOS==_VOS_UNKNOWN)
    {
        return "VOS_UNKNOWN";
    }

    QString sResult="";

    if(nFileOS&_VOS_DOS)
    {
        if(sResult!="")
        {
            sResult+="|";
        }

        sResult+="VOS_DOS";
    }

    if(nFileOS&_VOS_NT)
    {
        if(sResult!="")
        {
            sResult+="|";
        }

        sResult+="VOS_NT";
    }

    if(nFileOS&_VOS__WINDOWS32)
    {
        if(sResult!="")
        {
            sResult+="|";
        }

        sResult+="VOS__WINDOWS32";
    }

    return sResult;
}

QString VersionResource::fileTypeToString(unsigned int nFileType)
{
    if(nFileType==_VFT_UNKNOWN)
    {
        return "VFT_UNKNOWN";
    }
    else if(nFileType==_VFT_DLL)
    {
        return "VFT_DLL";
    }
    else if(nFileType==_VFT_DRV)
    {
        return "VFT_DRV";
    }
    else if(nFileType==_VFT_FONT)
    {
        return "VFT_FONT";
    }
    else if(nFileType==_VFT_STATIC_LIB)
    {
        return "VFT_STATIC_LIB";
    }
    else if(nFileType==_VFT_VXD)
    {
        return "VFT_VXD";
    }

    return "";
}

unsigned int VersionResource::getNextHeaderOffset(unsigned int nOffset)
{
    unsigned int nSize=nOffset+sizeof(_VS_VERSION_INFO);
    QString sString=QString::fromUtf16((ushort *)(pbaData->data()+nSize));
    //    nSize+=(wcslen((ushort *)(pbaData->data()+nSize))+1)*sizeof(short);
    nSize+=(sString.length()+1)*sizeof(short);
    nSize=ALIGN_UP(nSize,4);
    return nSize;
}

unsigned int VersionResource::getFixedFileInfo_Signature()
{
    return ((__VS_FIXEDFILEINFO *)(pbaData->data()+getNextHeaderOffset(0)))->dwSignature;
}

unsigned int VersionResource::getFixedFileInfo_StrucVersion()
{
    return ((__VS_FIXEDFILEINFO *)(pbaData->data()+getNextHeaderOffset(0)))->dwStrucVersion;
}

unsigned int VersionResource::getFixedFileInfo_FileVersionMS()
{
    return ((__VS_FIXEDFILEINFO *)(pbaData->data()+getNextHeaderOffset(0)))->dwFileVersionMS;
}

unsigned int VersionResource::getFixedFileInfo_FileVersionLS()
{
    return ((__VS_FIXEDFILEINFO *)(pbaData->data()+getNextHeaderOffset(0)))->dwFileVersionLS;
}

unsigned int VersionResource::getFixedFileInfo_ProductVersionMS()
{
    return ((__VS_FIXEDFILEINFO *)(pbaData->data()+getNextHeaderOffset(0)))->dwProductVersionMS;
}

unsigned int VersionResource::getFixedFileInfo_ProductVersionLS()
{
    return ((__VS_FIXEDFILEINFO *)(pbaData->data()+getNextHeaderOffset(0)))->dwProductVersionLS;
}

unsigned int VersionResource::getFixedFileInfo_FileFlagsMask()
{
    return ((__VS_FIXEDFILEINFO *)(pbaData->data()+getNextHeaderOffset(0)))->dwFileFlagsMask;
}

unsigned int VersionResource::getFixedFileInfo_FileFlags()
{
    return ((__VS_FIXEDFILEINFO *)(pbaData->data()+getNextHeaderOffset(0)))->dwFileFlags;
}

unsigned int VersionResource::getFixedFileInfo_FileOS()
{
    return ((__VS_FIXEDFILEINFO *)(pbaData->data()+getNextHeaderOffset(0)))->dwFileOS;
}

unsigned int VersionResource::getFixedFileInfo_FileType()
{
    return ((__VS_FIXEDFILEINFO *)(pbaData->data()+getNextHeaderOffset(0)))->dwFileType;
}

unsigned int VersionResource::getFixedFileInfo_FileSubtype()
{
    return ((__VS_FIXEDFILEINFO *)(pbaData->data()+getNextHeaderOffset(0)))->dwFileSubtype;
}

unsigned int VersionResource::getFixedFileInfo_FileDateMS()
{
    return ((__VS_FIXEDFILEINFO *)(pbaData->data()+getNextHeaderOffset(0)))->dwFileDateMS;
}

unsigned int VersionResource::getFixedFileInfo_FileDateLS()
{
    return ((__VS_FIXEDFILEINFO *)(pbaData->data()+getNextHeaderOffset(0)))->dwFileDateLS;
}

bool VersionResource::isChildrenPresent()
{
    int nDelta=(int)getLength(0)-(int)getNextHeaderOffset(0)+sizeof(__VS_FIXEDFILEINFO);

    return nDelta>0;

}
unsigned int VersionResource::getChildrenOffset()
{
    return ALIGN_UP((int)getNextHeaderOffset(0)+sizeof(__VS_FIXEDFILEINFO),4);
}
