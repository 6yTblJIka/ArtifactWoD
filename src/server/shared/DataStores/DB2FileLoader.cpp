////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "DB2FileLoader.h"

DB2FileLoader::DB2FileLoader()
{
    data = NULL;
    fieldsOffset = NULL;
}

bool DB2FileLoader::Load(const char *filename, const char *fmt)
{
    uint32 header = 48;
    if (data)
    {
        delete [] data;
        data=NULL;
    }

    FILE * f = fopen(filename, "rb");
    if (!f)
        return false;

    if (fread(&header, 4, 1, f) != 1)                        // Signature
    {
        fclose(f);
        return false;
    }

    EndianConvert(header);

    if (header != 0x32424457)
    {
        fclose(f);
        return false;                                       //'WDB2'
    }

    if (fread(&recordCount, 4, 1, f) != 1)                 // Number of records
    {
        fclose(f);
        return false;
    }

    EndianConvert(recordCount);

    if (fread(&fieldCount, 4, 1, f) != 1)                 // Number of fields
    {
        fclose(f);
        return false;
    }

    EndianConvert(fieldCount);

    if (fread(&recordSize, 4, 1, f) != 1)                 // Size of a record
    {
        fclose(f);
        return false;
    }

    EndianConvert(recordSize);

    if (fread(&stringSize, 4, 1, f) != 1)                 // String size
    {
        fclose(f);
        return false;
    }

    EndianConvert(stringSize);

    /* NEW WDB2 FIELDS*/
    if (fread(&tableHash, 4, 1, f) != 1)                  // Table hash
    {
        fclose(f);
        return false;
    }

    EndianConvert(tableHash);

    if (fread(&build, 4, 1, f) != 1)                     // Build
    {
        fclose(f);
        return false;
    }

    EndianConvert(build);

    if (fread(&unk1, 4, 1, f) != 1)                     // Unknown WDB2
    {
        fclose(f);
        return false;
    }

    EndianConvert(unk1);

    if (build > 12880)
    {
        if (fread(&unk2, 4, 1, f) != 1)                // Unknown WDB2
        {
            fclose(f);
            return false;
        }
        EndianConvert(unk2);

        if (fread(&maxIndex, 4, 1, f) != 1)                           // MaxIndex WDB2
        {
            fclose(f);
            return false;
        }
        EndianConvert(maxIndex);

        if (fread(&locale, 4, 1, f) != 1)                             // Locales
        {
            fclose(f);
            return false;
        }
        EndianConvert(locale);

        if (fread(&unk5, 4, 1, f) != 1)                               // Unknown WDB2
        {
            fclose(f);
            return false;
        }
        EndianConvert(unk5);
    }

    if (maxIndex != 0)
    {
        int32 diff = maxIndex - unk2 + 1;
        fseek(f, diff * 4 + diff * 2, SEEK_CUR);    // diff * 4: an index for rows, diff * 2: a memory allocation bank
    }

    fieldsOffset = new uint32[fieldCount];
    fieldsOffset[0] = 0;
    for (uint32 i = 1; i < fieldCount; i++)
    {
        fieldsOffset[i] = fieldsOffset[i - 1];
        if (fmt[i - 1] == 'b' || fmt[i - 1] == 'X')
            fieldsOffset[i] += 1;
        else
            fieldsOffset[i] += 4;
    }

    data = new unsigned char[recordSize*recordCount+stringSize];
    stringTable = data + recordSize*recordCount;

    if (fread(data, recordSize * recordCount + stringSize, 1, f) != 1)
    {
        fclose(f);
        return false;
    }

    fclose(f);
    return true;
}

DB2FileLoader::~DB2FileLoader()
{
    if (data)
        delete [] data;
    if (fieldsOffset)
        delete [] fieldsOffset;
}

DB2FileLoader::Record DB2FileLoader::getRecord(size_t id)
{
    assert(data);
    return Record(*this, data + id*recordSize);
}

uint32 DB2FileLoader::GetFormatRecordSize(const char * format, int32* index_pos)
{
    uint32 recordsize = 0;
    int32 i = -1;
    for (uint32 x=0; format[x]; ++x)
    {
        switch(format[x])
        {
            case FT_FLOAT:
            case FT_INT:
                recordsize += 4;
                break;
            case FT_STRING:
                recordsize += sizeof(LocalizedString*);
                break;
            case FT_SORT:
                i = x;
                break;
            case FT_INDEX:
                i = x;
                recordsize += 4;
                break;
            case FT_BYTE:
                recordsize += 1;
                break;
        }
    }

    if (index_pos)
        *index_pos = i;

    return recordsize;
}

uint32 DB2FileLoader::GetFormatStringsFields(const char * format)
{
    uint32 stringfields = 0;
    for (uint32 x=0; format[x]; ++x)
        if (format[x] == FT_STRING)
            ++stringfields;

    return stringfields;
}

static char const* const nullStr = "";

char* DB2FileLoader::AutoProduceData(const char* format, uint32& records, char**& indexTable, std::set<LocalizedString*> & p_LocalizedString)
{

    typedef char * ptr;
    if (strlen(format) != fieldCount)
        return NULL;

    //get struct size and index pos
    int32 i;
    uint32 recordsize=GetFormatRecordSize(format, &i);

    if (i >= 0)
    {
        uint32 maxi = 0;
        //find max index
        for (uint32 y = 0; y < recordCount; y++)
        {
            uint32 ind=getRecord(y).getUInt(i);
            if (ind>maxi)
                maxi = ind;
        }

        ++maxi;
        records = maxi;
        indexTable = new ptr[maxi];
        memset(indexTable, 0, maxi * sizeof(ptr));
    }
    else
    {
        records = recordCount;
        indexTable = new ptr[recordCount];
    }

    char* dataTable = new char[recordCount * recordsize];

    uint32 offset=0;

    for (uint32 y =0; y < recordCount; y++)
    {
        if (i>=0)
        {
            indexTable[getRecord(y).getUInt(i)] = &dataTable[offset];
        }
        else
            indexTable[y] = &dataTable[offset];

        for (uint32 x = 0; x < fieldCount; x++)
        {
            switch(format[x])
            {
                case FT_FLOAT:
                    *((float*)(&dataTable[offset])) = getRecord(y).getFloat(x);
                    offset += 4;
                    break;
                case FT_INDEX:
                case FT_INT:
                    *((uint32*)(&dataTable[offset])) = getRecord(y).getUInt(x);
                    offset += 4;
                    break;
                case FT_BYTE:
                    *((uint8*)(&dataTable[offset])) = getRecord(y).getUInt8(x);
                    offset += 1;
                    break;
                case FT_STRING:
                    LocalizedString* l_Str = new LocalizedString(nullStr);
                    *((LocalizedString**)(&dataTable[offset])) = l_Str;   // will be replaces non-empty or "" strings in AutoProduceStrings
                    p_LocalizedString.emplace(l_Str);
                    offset += sizeof(LocalizedString*);
                    break;
            }
        }
    }

    return dataTable;
}

char* DB2FileLoader::AutoProduceStringsArrayHolders(const char* format, char* /*dataTable*/, uint32 /*p_Locale*/)
{
    if (strlen(format) != fieldCount)
        return NULL;

    // we store flat holders pool as single memory block
    size_t stringFields = GetFormatStringsFields(format);
    // each string field at load have array of string for each locale
    size_t stringHolderSize = sizeof(char*) * TOTAL_LOCALES;
    size_t stringHoldersRecordPoolSize = stringFields * stringHolderSize;
    size_t stringHoldersPoolSize = stringHoldersRecordPoolSize * recordCount;

    char* stringHoldersPool = new char[stringHoldersPoolSize];

    // DB2 strings expected to have at least empty string
    for (size_t i = 0; i < stringHoldersPoolSize / sizeof(char*); ++i)
        ((char const**)stringHoldersPool)[i] = nullStr;

    uint32 offset=0;

    // assign string holders to string field slots
    for (uint32 y = 0; y < recordCount; y++)
    {
        uint32 stringFieldNum = 0;

        for (uint32 x = 0; x < fieldCount; x++)
            switch(format[x])
            {
                case FT_FLOAT:
                case FT_INDEX:
                case FT_INT:
                    offset += 4;
                    break;
                case FT_BYTE:
                    offset += 1;
                    break;
                case FT_STRING:
                {
                    /*
                    uhm WTF ? THis breaks every fields, gets properly Generated later @ AutoProduceStrings
                    LocalizedString ** l_PtrPtr = ((LocalizedString**)(&dataTable[offset]));
                    char ** l_StrPtrMemoryPos = (char **)(&(*l_PtrPtr)->Str[p_Locale]);
                    *l_StrPtrMemoryPos = &stringHoldersPool[stringHoldersRecordPoolSize * y + stringHolderSize*stringFieldNum];
                    */

                    ++stringFieldNum;
                    offset += sizeof(LocalizedString*);
                    break;
                }
                case FT_NA:
                case FT_NA_BYTE:
                case FT_SORT:
                    break;
                default:
                    assert(false && "unknown format character");
        }
    }

    //send as char* for store in char* pool list for free at unload
    return stringHoldersPool;
}

char* DB2FileLoader::AutoProduceStrings(const char* format, char* dataTable, uint32 p_Locale)
{
    if (strlen(format) != fieldCount)
        return NULL;

    char* stringPool= new char[stringSize];
    memcpy(stringPool, stringTable, stringSize);

    uint32 offset = 0;

    for (uint32 y =0; y < recordCount; y++)
    {
        for (uint32 x = 0; x < fieldCount; x++)
            switch(format[x])
        {
            case FT_FLOAT:
            case FT_INDEX:
            case FT_INT:
                offset += 4;
                break;
            case FT_BYTE:
                offset += 1;
                break;
            case FT_STRING:
            {
                // fill only not filled entries
                LocalizedString* l_PtrPtr = *((LocalizedString**)(&dataTable[offset]));
                if (l_PtrPtr->Str[p_Locale] == nullStr)
                {
                    const char* st = getRecord(y).getString(x);
                    l_PtrPtr->Str[p_Locale] = stringPool + (st - (const char*)stringTable);
                }

                offset += sizeof(LocalizedString*);
                break;
            }
        }
    }

    return stringPool;
}
