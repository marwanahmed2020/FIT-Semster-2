#ifndef __PROGTEST__
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <iostream>
#include <initializer_list>
#endif /* __PROGTEST__ */

class CFile
{
private:
    static const size_t CHUNK_SIZE = 4096;

    struct FileData
    {
        struct Chunk
        {
            uint8_t data[CHUNK_SIZE];
            size_t used;
            int referenceCount;

            Chunk() : used(0), referenceCount(1) {}
        };

        Chunk** buffer;
        size_t currentSize;
        size_t currentCapacity;
        size_t chunkCount;
        int referenceCount;

        FileData()
            : buffer(nullptr), currentSize(0), currentCapacity(0), chunkCount(0), referenceCount(1) {}

        ~FileData()
        {
            for (size_t index = 0; index < chunkCount; ++index)
                releaseChunk(buffer[index]);
            delete[] buffer;
        }
    };

    FileData* fileData;
    size_t currentPosition;

    struct Version
    {
        FileData* snapshotData;
        size_t snapshotPosition;
    };

    Version* versionHistory;
    int versionCount;
    int versionCapacity;

public:
    CFile() : fileData(new FileData()), currentPosition(0), versionHistory(nullptr), versionCount(0), versionCapacity(0) {}

    CFile(const CFile& other)
    {
        fileData = other.fileData;
        fileData->referenceCount++;
        currentPosition = other.currentPosition;

        versionCount = other.versionCount;
        versionCapacity = other.versionCapacity;
        versionHistory = versionCapacity ? new Version[versionCapacity] : nullptr;

        for (int index = 0; index < versionCount; index++)
        {
            versionHistory[index] = other.versionHistory[index];
            versionHistory[index].snapshotData->referenceCount++;
        }
    }

    ~CFile()
    {
        clearVersionHistory();
        releaseFileData(fileData);
    }

    CFile& operator=(const CFile& other)
    {
        if (this == &other) return *this;

        clearVersionHistory();
        releaseFileData(fileData);

        fileData = other.fileData;
        fileData->referenceCount++;
        currentPosition = other.currentPosition;

        versionCount = other.versionCount;
        versionCapacity = other.versionCapacity;
        versionHistory = versionCapacity ? new Version[versionCapacity] : nullptr;

        for (int index = 0; index < versionCount; index++)
        {
            versionHistory[index] = other.versionHistory[index];
            versionHistory[index].snapshotData->referenceCount++;
        }

        return *this;
    }

    bool seek(size_t offset)
    {
        if (offset > fileData->currentSize) return false;
        currentPosition = offset;
        return true;
    }

    size_t read(uint8_t dst[], size_t bytes)
    {
        if (currentPosition >= fileData->currentSize || bytes == 0) return 0;

        size_t availableBytes = fileData->currentSize - currentPosition;
        if (bytes > availableBytes) bytes = availableBytes;

        size_t copiedBytes = 0;
        size_t chunkIndex = 0;
        size_t chunkOffset = 0;
        locatePosition(currentPosition, chunkIndex, chunkOffset);

        while (copiedBytes < bytes)
        {
            FileData::Chunk* currentChunk = fileData->buffer[chunkIndex];
            size_t availableInChunk = currentChunk->used - chunkOffset;
            size_t bytesNow = bytes - copiedBytes;
            if (bytesNow > availableInChunk)
                bytesNow = availableInChunk;

            for (size_t index = 0; index < bytesNow; ++index)
                dst[copiedBytes + index] = currentChunk->data[chunkOffset + index];

            copiedBytes += bytesNow;
            ++chunkIndex;
            chunkOffset = 0;
        }

        currentPosition += copiedBytes;
        return copiedBytes;
    }

    size_t write(const uint8_t src[], size_t bytes)
    {
        if (bytes == 0) return 0;

        detachSharedData();

        size_t writtenBytes = 0;

        while (writtenBytes < bytes)
        {
            if (currentPosition < fileData->currentSize)
            {
                size_t chunkIndex = 0;
                size_t chunkOffset = 0;
                locatePosition(currentPosition, chunkIndex, chunkOffset);
                detachChunk(chunkIndex);

                FileData::Chunk* currentChunk = fileData->buffer[chunkIndex];
                size_t availableInChunk = currentChunk->used - chunkOffset;
                size_t bytesNow = bytes - writtenBytes;
                if (bytesNow > availableInChunk)
                    bytesNow = availableInChunk;

                for (size_t index = 0; index < bytesNow; ++index)
                    currentChunk->data[chunkOffset + index] = src[writtenBytes + index];

                currentPosition += bytesNow;
                writtenBytes += bytesNow;
            }
            else
            {
                appendBytes(src, writtenBytes, bytes);
            }
        }

        if (currentPosition > fileData->currentSize)
            fileData->currentSize = currentPosition;

        return writtenBytes;
    }

    void truncate()
    {
        if (currentPosition >= fileData->currentSize) return;

        detachSharedData();

        size_t chunkIndex = 0;
        size_t chunkOffset = 0;
        locatePosition(currentPosition, chunkIndex, chunkOffset);

        if (chunkOffset == 0)
        {
            for (size_t index = chunkIndex; index < fileData->chunkCount; ++index)
                releaseChunk(fileData->buffer[index]);
            fileData->chunkCount = chunkIndex;
        }
        else
        {
            detachChunk(chunkIndex);
            FileData::Chunk* currentChunk = fileData->buffer[chunkIndex];

            for (size_t index = chunkIndex + 1; index < fileData->chunkCount; ++index)
                releaseChunk(fileData->buffer[index]);

            fileData->chunkCount = chunkIndex + 1;
            currentChunk->used = chunkOffset;
        }

        fileData->currentSize = currentPosition;
    }

    size_t fileSize() const
    {
        return fileData->currentSize;
    }

    void addVersion()
    {
        if (versionCount == versionCapacity)
        {
            int newVersionCapacity = versionCapacity ? versionCapacity * 2 : 4;
            Version* newHistory = new Version[newVersionCapacity];
            for (int index = 0; index < versionCount; index++)
                newHistory[index] = versionHistory[index];
            delete[] versionHistory;
            versionHistory = newHistory;
            versionCapacity = newVersionCapacity;
        }

        versionHistory[versionCount].snapshotData = fileData;
        versionHistory[versionCount].snapshotPosition = currentPosition;
        fileData->referenceCount++;
        versionCount++;
    }

    bool undoVersion()
    {
        if (versionCount == 0) return false;

        versionCount--;
        releaseFileData(fileData);
        fileData = versionHistory[versionCount].snapshotData;
        currentPosition = versionHistory[versionCount].snapshotPosition;

        return true;
    }

private:

    static FileData::Chunk* createChunk()
    {
        return new FileData::Chunk();
    }

    static void releaseChunk(FileData::Chunk* chunk)
    {
        if (chunk && --chunk->referenceCount == 0)
            delete chunk;
    }

    static void releaseFileData(FileData* dataToRelease)
    {
        if (dataToRelease && --dataToRelease->referenceCount == 0)
            delete dataToRelease;
    }

    void detachSharedData()
    {
        if (fileData->referenceCount == 1) return;

        FileData* detachedData = new FileData();
        detachedData->currentSize = fileData->currentSize;
        detachedData->currentCapacity = fileData->chunkCount ? fileData->chunkCount + 4 : 4;
        while (detachedData->currentCapacity < fileData->chunkCount)
            detachedData->currentCapacity *= 2;
        detachedData->chunkCount = fileData->chunkCount;
        detachedData->buffer = detachedData->currentCapacity ? new FileData::Chunk*[detachedData->currentCapacity] : nullptr;

        for (size_t index = 0; index < fileData->chunkCount; ++index)
        {
            detachedData->buffer[index] = fileData->buffer[index];
            detachedData->buffer[index]->referenceCount++;
        }

        fileData->referenceCount--;
        fileData = detachedData;
    }

    void detachChunk(size_t chunkIndex)
    {
        FileData::Chunk* chunk = fileData->buffer[chunkIndex];
        if (chunk->referenceCount == 1)
            return;

        FileData::Chunk* detachedChunk = createChunk();
        detachedChunk->used = chunk->used;
        for (size_t index = 0; index < chunk->used; ++index)
            detachedChunk->data[index] = chunk->data[index];

        --chunk->referenceCount;
        fileData->buffer[chunkIndex] = detachedChunk;
    }

    void ensureChunkCapacity(size_t requiredCapacity)
    {
        if (requiredCapacity <= fileData->currentCapacity)
            return;

        size_t newCapacity = fileData->currentCapacity ? fileData->currentCapacity * 2 : 4;
        while (newCapacity < requiredCapacity)
            newCapacity *= 2;

        FileData::Chunk** newBuffer = new FileData::Chunk*[newCapacity];
        for (size_t index = 0; index < fileData->chunkCount; ++index)
            newBuffer[index] = fileData->buffer[index];

        delete[] fileData->buffer;
        fileData->buffer = newBuffer;
        fileData->currentCapacity = newCapacity;
    }

    void appendBytes(const uint8_t sourceBuffer[], size_t& alreadyWritten, size_t totalToWrite)
    {
        if (fileData->chunkCount == 0 || fileData->buffer[fileData->chunkCount - 1]->used == CHUNK_SIZE)
        {
            ensureChunkCapacity(fileData->chunkCount + 1);
            fileData->buffer[fileData->chunkCount] = createChunk();
            ++fileData->chunkCount;
        }
        else
        {
            detachChunk(fileData->chunkCount - 1);
        }

        FileData::Chunk* lastChunk = fileData->buffer[fileData->chunkCount - 1];
        size_t freeBytesInChunk = CHUNK_SIZE - lastChunk->used;
        size_t bytesNow = totalToWrite - alreadyWritten;
        if (bytesNow > freeBytesInChunk)
            bytesNow = freeBytesInChunk;

        for (size_t index = 0; index < bytesNow; ++index)
            lastChunk->data[lastChunk->used + index] = sourceBuffer[alreadyWritten + index];

        lastChunk->used += bytesNow;
        fileData->currentSize += bytesNow;
        currentPosition += bytesNow;
        alreadyWritten += bytesNow;
    }

    void clearChunksInCurrentState()
    {
        for (size_t index = 0; index < fileData->chunkCount; ++index)
            releaseChunk(fileData->buffer[index]);
        fileData->chunkCount = 0;
    }

    void clearVersionHistory()
    {
        for (int index = 0; index < versionCount; ++index)
            releaseFileData(versionHistory[index].snapshotData);
        delete[] versionHistory;
        versionHistory = nullptr;
        versionCount = 0;
        versionCapacity = 0;
    }

    void copyFrom(const CFile& other)
    {
        fileData = other.fileData;
        fileData->referenceCount++;
        currentPosition = other.currentPosition;

        versionCount = other.versionCount;
        versionCapacity = other.versionCapacity;
        versionHistory = versionCapacity ? new Version[versionCapacity] : nullptr;

        for (int index = 0; index < versionCount; index++)
        {
            versionHistory[index] = other.versionHistory[index];
            versionHistory[index].snapshotData->referenceCount++;
        }
    }

    void locatePosition(size_t positionToLocate, size_t& chunkIndex, size_t& offsetInsideChunk) const
    {
        size_t passedBytes = 0;
        chunkIndex = 0;

        while (chunkIndex < fileData->chunkCount
               && passedBytes + fileData->buffer[chunkIndex]->used <= positionToLocate)
        {
            passedBytes += fileData->buffer[chunkIndex]->used;
            ++chunkIndex;
        }

        offsetInsideChunk = positionToLocate - passedBytes;
    }

    void clearAll()
    {
        if (versionHistory != nullptr)
        {
            for (int index = 0; index < versionCount; ++index)
                releaseFileData(versionHistory[index].snapshotData);
            delete[] versionHistory;
        }

        releaseFileData(fileData);
        versionHistory = nullptr;
        fileData = nullptr;
        versionCount = 0;
        versionCapacity = 0;
        currentPosition = 0;
    }
};

#ifndef __PROGTEST__
bool writeTest(CFile& fileInstance, const std::initializer_list<uint8_t>& expectedData, size_t expectedWrittenBytes)
{
    return fileInstance.write(expectedData.begin(), expectedData.size()) == expectedWrittenBytes;
}

bool readTest(CFile& fileInstance, const std::initializer_list<uint8_t>& expectedData, size_t requestedReadBytes)
{
    uint8_t readBuffer[100];
    uint32_t readIndex = 0;

    if (fileInstance.read(readBuffer, requestedReadBytes) != expectedData.size())
        return false;
    for (auto expectedByte : expectedData)
        if (readBuffer[readIndex++] != expectedByte)
            return false;
    return true;
}

int main()
{
    CFile firstFile;

    assert(writeTest(firstFile, {10, 20, 30}, 3));
    assert(firstFile.fileSize() == 3);
    assert(writeTest(firstFile, {60, 70, 80}, 3));
    assert(firstFile.fileSize() == 6);
    assert(firstFile.seek(2));
    assert(writeTest(firstFile, {5, 4}, 2));
    assert(firstFile.fileSize() == 6);
    assert(firstFile.seek(1));
    assert(readTest(firstFile, {20, 5, 4, 70, 80}, 7));
    assert(firstFile.seek(3));
    firstFile.addVersion();
    assert(firstFile.seek(6));
    assert(writeTest(firstFile, {100, 101, 102, 103}, 4));
    firstFile.addVersion();
    assert(firstFile.seek(5));
    CFile copiedFile(firstFile);
    firstFile.truncate();
    assert(firstFile.seek(0));
    assert(readTest(firstFile, {10, 20, 5, 4, 70}, 20));
    assert(firstFile.undoVersion());
    assert(firstFile.seek(0));
    assert(readTest(firstFile, {10, 20, 5, 4, 70, 80, 100, 101, 102, 103}, 20));
    assert(firstFile.undoVersion());
    assert(firstFile.seek(0));
    assert(readTest(firstFile, {10, 20, 5, 4, 70, 80}, 20));
    assert(!firstFile.seek(100));
    assert(writeTest(copiedFile, {200, 210, 220}, 3));
    assert(copiedFile.seek(0));
    assert(readTest(copiedFile, {10, 20, 5, 4, 70, 200, 210, 220, 102, 103}, 20));
    assert(copiedFile.undoVersion());
    assert(copiedFile.undoVersion());
    assert(readTest(copiedFile, {4, 70, 80}, 20));
    assert(!copiedFile.undoVersion());
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */