class Chunk
{
    int fd;           // File descriptor where data is stored
    int starting_pos; // Starting position in file
    int nb_element;   // Current number of element in the chunk
    void *content;    // Buffer for reading data (could be nullptr when data is not loaded)

public:
    Chunk(int fd, int starting_pos);
    ~Chunk();

    virtual void load();   // Load the content
    virtual void unload(); // Unload the content
    virtual int readInt(int chunk_pos);
    virtual double readDouble(int chunk_pos);
    virtual void writeInt(int chunk_pos, int value);
    virtual void writeDouble(int chunk_pos, double value);
};

class IntegerChunk : public Chunk
{
};

class DoubleChunk : public Chunk
{
};