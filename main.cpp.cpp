#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <bitset>
#include <unordered_map>

using namespace std;

class Node
{
private:
    void traversehelper(const Node *node, string &&code) const
    {
        node->traverse(move(code));
    }
public:
    int freq;
    char data;
    const Node *left;
    const Node *right;
    Node(char d, int f)
    {
        freq = f;
        data = d;
        left = NULL;
        right = NULL;
    }
    Node(const Node *l, const Node *r)
    {
        data = 0;
        freq = l->freq + r->freq;
        left = l;
        right = r;
    }
    bool operator<(const Node &a) const
    {
        return freq > a.freq;
    }
    void traverse(string &&code) const
    {
        if (left != NULL)
        {
            left->traversehelper(left, move(code + '0'));
            right->traversehelper(right, move(code + '1'));
        }
    }
};

class PriorityQueue
{
private:
    Node **heapArray;
    int maxsize;
    int currentsize;
public:
    PriorityQueue(int size)
    {
        maxsize = size;
        currentsize = 0;
        heapArray = new Node *[maxsize];
    }
    ~PriorityQueue()
    {
        delete[] heapArray;
    }
    void push(const Node &node)
    {
        if (currentsize == maxsize)
        {
            cout << "Priority Queue is full!";
            return;
        }
        int currentIndex = currentsize++;
        int parentIndex = (currentIndex - 1) / 2;
        while ((currentIndex > 0) && (*heapArray[parentIndex] < node))
        {
            heapArray[currentIndex] = heapArray[parentIndex];
            currentIndex = parentIndex;
            parentIndex = (currentIndex - 1) / 2;
        }
        heapArray[currentIndex] = new Node(node);
    }
    void pop()
    {
        if (currentsize == 0)
        {
            cout << "Priority Queue is empty!";
            return;
        }
        Node *lastNode = heapArray[--currentsize];
        int currentIndex = 0;
        int childIndex = 1;
        while (childIndex < currentsize)
        {
            if ((childIndex < currentsize - 1) && (*heapArray[childIndex] < *heapArray[childIndex + 1]))
            {
                childIndex++;
            }
            if (*heapArray[childIndex] < *lastNode)
            {
                break;
            }
            heapArray[currentIndex] = heapArray[childIndex];
            currentIndex = childIndex;
            childIndex = 2 * childIndex + 1;
        }
        heapArray[currentIndex] = lastNode;
    }
    Node top()
    {
        if (currentsize == 0)
        {
            cout << "Priority Queue is empty";
            return Node(0, 0);
        }
        return *heapArray[0];
    }
    int size()
    {
        return currentsize;
    }
};

class HuffmanCoding
{
private:
    unordered_map<char, string> huffmancodes;
    void generatecodes(const Node *root, string &&code)
    {
        if (root != nullptr)
        {
            if (root->left != nullptr)
            {
                generatecodes(root->left, move(code + '0'));
                generatecodes(root->right, move(code + '1'));
            }
            else
            {
                huffmancodes[root->data] = code;
            }
        }
    }
public:
    char ch;
    string code;
    string buffer;
    bitset<8> byte;
    void compressfile(const string &inputfile, const string &outputfile)
    {
        // Use absolute paths
        string scriptDirectory = "C:/xampp/cgi-bin/"; // Adjust this accordingly
        string absoluteInputFile = scriptDirectory + inputfile;
        string absoluteOutputFile = scriptDirectory + outputfile;

        // Open input file with absolute path
        ifstream infile(absoluteInputFile, ios::binary);
        if (!infile)
        {
            cerr << "Error opening input file: " << absoluteInputFile << "<br>";
            cout << "<h1>Error opening input file</h1>";
            return;
        }
        PriorityQueue pq(256);
        int frequency[256] = {0};
        while (infile.get(ch))
        {
            frequency[char(ch)]++;
        }
        for (int i = 0; i < 256; i++)
        {
            if (frequency[i] != 0)
            {
                pq.push(Node(char(i), frequency[i]));
            }
        }
        while (pq.size() > 1)
        {
            Node *c0 = new Node(pq.top());
            pq.pop();
            Node *c1 = new Node(pq.top());
            pq.pop();
            pq.push(Node(c0, c1));
        }
        generatecodes(pq.top().left, move(code));
        // Open output file with absolute path
        ofstream outfile(absoluteOutputFile, ios::binary);
        if (!outfile)
        {
            cerr << "Error opening output file: " << absoluteOutputFile << "<br>";
            return;
        }
        infile.clear();
        infile.seekg(0, ios::beg);
        while (infile.get(ch))
        {
            buffer += huffmancodes[ch];
            while (buffer.length() >= 8)
            {
                for (int i = 0; i < 8; i++)
                {
                    byte[i] = buffer[i] - '0';
                }
                outfile.put(char(byte.to_ulong()));
                buffer = buffer.substr(8);
            }
        }
        if (!buffer.empty())
        {
            for (size_t i = 0; i < buffer.length(); i++)
            {
                byte[i] = buffer[i] - '0';
            }
            outfile.put(char(byte.to_ulong()));
        }
        infile.close();
        outfile.close();
        // Display result
            cout << "<h1>Compressed!</h1>";
            cout << "<p>Compression complete.</p>";
    }
};

int cgiMain()
{
    cout << "Content-type: text/html\r\n\r\n";
    // Print HTML header with styling and JavaScript
    cout << "<html lang=\"en\">\n<head>\n";
    cout << "    <script src=\"https://cdnjs.cloudflare.com/ajax/libs/three.js/r121/three.min.js\"></script>\n";
    cout << "    <script src=\"https://cdn.jsdelivr.net/npm/vanta@latest/dist/vanta.net.min.js\"></script>\n";
    cout << "    <script>\n";
    cout << "        document.addEventListener(\"DOMContentLoaded\", function(){\n";
    cout << "            VANTA.NET({\n";
    cout << "                el: \"#wrapper\",\n";
    cout << "                mouseControls: true,\n";
    cout << "                touchControls: true,\n";
    cout << "                gyroControls: false,\n";
    cout << "                minHeight: 200.00,\n";
    cout << "                minWidth: 200.00,\n";
    cout << "                scale: 1.00,\n";
    cout << "                scaleMobile: 1.00,\n";
    cout << "                color: 0x8919a0,\n";
    cout << "                backgroundColor: 0x0,\n";
    cout << "                maxDistance: 22.00\n";
    cout << "            })\n";
    cout << "        });\n";
    cout << "    </script>\n";
    cout << "    <title>File Compressor</title>\n";
    cout << "    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\n";
    cout << "    <link rel=\"preconnect\" href=\"https://fonts.googleapis.com\">\n";
    cout << "    <link rel=\"preconnect\" href=\"https://fonts.gstatic.com\" crossorigin>\n";
    cout << "    <link href=\"https://fonts.googleapis.com/css2?family=Kalam:wght@300;400;700&family=Oswald:wght@300;400;500;600&display=swap\" rel=\"stylesheet\">\n";
    cout << "    <style type=\"text/css\">\n";
    cout << "        html, body {\n";
    cout << "            height: 100%;\n";
    cout << "            margin: 0;\n";
    cout << "            font-family: 'Oswald', sans-serif;\n";
    cout << "        }\n";
    cout << "        #wrapper {\n";
    cout << "            min-height: 100%;\n";
    cout << "        }\n";
    cout << "        .heading{\n";
    cout << "            font-family: 'Kalam', cursive;\n";
    cout << "        }\n";
    cout << "        .bton{\n";
    cout << "            font-weight:600;\n";
    cout << "            font-size: 16px;\n";
    cout << "            background: white;\n";
    cout << "            color: black;\n";
    cout << "            border:transparent;\n";
    cout << "            padding: 8px 25px;\n";
    cout << "            transition: transform 0.5s;\n";
    cout << "        }\n";
    cout << "        .bton:hover{\n";
    cout << "            font-weight:600;\n";
    cout << "            cursor:pointer;\n";
    cout << "            background:transparent;\n";
    cout << "            color:white;\n";
    cout << "            text-decoration:none;\n";
    cout << "            transform: translateY(-8px);\n";
    cout << "        }\n";
    cout << "        .file-input {\n";
    cout << "            font-size: 16px;\n";
    cout << "            width: 170px;\n";
    cout << "            height: 25px;\n";
    cout << "            margin-left: 44%;\n";
    cout << "            background: white;\n";
    cout << "            color: #a0a0a0;\n";
    cout << "        }\n";
    cout << "    </style>\n";
    cout << "    <!--[if lte IE 6]>\n";
    cout << "    <style type=\"text/css\">\n";
    cout << "        #container {\n";
    cout << "            height: 100%;\n";
    cout << "        }\n";
    cout << "    </style>\n";
    cout << "    <![endif]-->\n";
    cout << "</head>\n<body style=\"background-color: black; align-items: center; margin-top:-30px;\">\n";
    cout << "    <div style=\"color: white; text-align:center;\" id=\"wrapper\">\n";
    cout << "        <h1 style=\"font-size: 44px; font-weight:700; padding-top: 150px;\">FILE COMPRESSION</h1>\n";
    cout << "        <p class=\"heading\" style=\"margin-top:-10px;\">using</p>\n";
    cout << "        <h1 style=\"color:rgb(101 229 185 / 64%); font-weight:700; font-size: 64px; margin-top:-10px;\"><i>HUFFMAN CODING</i></h1>\n";
    // Handle form submission
    if (getenv("REQUEST_METHOD") && string(getenv("REQUEST_METHOD")) == "POST")
    {
        // Get input file from form
        char *lenstr = getenv("CONTENT_LENGTH");
        if (lenstr && atoi(lenstr) > 0)
        {
            int len = atoi(lenstr);
            char *postdata = new char[len + 1];
            cin.read(postdata, len);
            postdata[len] = '\0';
            // Extract input file path from form data
            string inputfile;
            char *filename_start = strstr(postdata, "filename=\"");
            if (filename_start != nullptr)
            {
                filename_start += 10; // Move to the first character of the filename
                char *filename_end = strchr(filename_start, '"'); // Find the end of the filename
                if (filename_end != nullptr)
                {
                    // Extract the filename
                    size_t filename_length = filename_end - filename_start;
                    inputfile = string(filename_start, filename_length);
                }
            }
            // Perform Huffman coding
            HuffmanCoding compressor;
            // string outputfile = "output.huf";
            compressor.compressfile(inputfile, inputfile+".huf");
        }
        else
        {
            cout << "<p>Error: No input provided</p>";
        }
    }
    else
    {
        cout << "        <form action=\"/index.cgi\" method=\"POST\" enctype=\"multipart/form-data\">\n";
        cout << "            <label style=\"margin-top:20px; margin-bottom:30px; font-weight:lighter; font-size:16px;\">Select any text-based file to compress it !</label>\n";
        cout << "            <input type=\"file\" name=\"file\" id=\"file\" class=\"file-input\">\n";
        cout << "            <button style=\"margin-top:30px;\" class=\"bton\" type=\"submit\">Upload</button><br>\n";
        // cout << "            <label style=\"margin-top:20px; font-weight:lighter; font-size:16px;\">Please wait after uploading so that your file gets compressed.</label>\n";
        cout << "        </form>\n";
    }
    
    return 0;
}

int main()
{
    return cgiMain();
}