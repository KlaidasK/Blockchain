#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <istream>
#include <iomanip>
#include <chrono>
#include <bitset>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <random>
#include <numeric>
#include <execution>
#include <ctime>
#include <format>

using std::string;
using std::cout;
using std::endl;
using std::cin;
using std::endl;
using std::vector;


string customHash2(const std::string& input) {
    unsigned long long K[64] = { 0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
                                0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
                                0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
                                0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
                                0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
                                0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
                                0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
                                0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 };

    int hash[8];
    int p = 0;

    for (int i = 0; i < 8; i++) {
        for (char c : input) {
            p = p + 3;
            hash[i] ^= (hash[i] << 5) + (hash[i] >> 2) + static_cast<unsigned long long>(c);
            hash[i] *= K[p % 64];
        }
    }


    std::stringstream ss;
    for (int i = 0; i < 8; i++) {
        ss << std::hex << std::setw(8) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
}

class User {

    private:
    double eur; //Valiutos atsitiktinis balansas
    string name; //Vardas
    public:
        string public_key; //viešasis hash raktas
        //Default konst'ius
        User(){
            this->public_key = {};
            this->name = "";
            this->eur = 0;
        }

        User(string v, string p, double e)
        {
            this->public_key = v;
            this->name = p;
            this->eur = e;
        }

        double geteur() const { return eur; }
        void seteur(double _value) { this->eur = _value; }
        string getname() const { return name; }
        void setname(string _value) { this->name = _value; }
        string getpublic_key() const { return public_key; }
        void setpublic_key(string _value) { this->public_key = _value; }

        ~User() { eur = 0; name.clear(); public_key.clear(); }
};

class Transaction {

    private:
         string HashID; //kitų transakcijos laukų hash'as

    public:
        string sender; //jo viešasis raktas
        string receiver; //jo viešasis raktas
        double suma; 

        //Default konst'ius
        Transaction() {
            string hash;
            this->sender = "";
            this->receiver = "";
            this->suma = 0;
            hash = sender + receiver + std::to_string(suma);
            this->HashID = customHash2(hash);
        }

        Transaction(string h, string s, string r, double e)
        {
            string hash;
            this->sender = s;
            this->receiver = r;
            this->suma = e;
            hash = sender + receiver + std::to_string(suma);
            this->HashID = customHash2(hash);
        }

        Transaction(string s, string r, double e) {
            string hash;
            this->sender = s;
            this->receiver = r;
            this->suma = e;
            hash = sender + receiver + std::to_string(suma);
            this->HashID = customHash2(hash);
        }

        string gethash() const { return HashID; }
        void sethash(string _value) { this->HashID = _value; }
        string getsender() const { return sender; }
        void setsender(string _value) { this->sender = _value; }
        string getreceiver() const { return receiver; }
        void setreceiver(string _value) { this->receiver = _value; }
        double getsuma() const { return suma; }
        void setsuma(double _value) { this->suma = _value; }

        ~Transaction() {
            HashID.clear();
            sender.clear();
            receiver.clear(); 
            suma = 0;
        }
};

class Block {
private:
    class Header {
    private:
        string Hash;
        string Prev_Block_Hash;
        std::string Timestamp; // Use ISO 8601 timestamp format
        double version;
        string Merkel_Root_Hash;
        int Nonce; //Atsitiktinio skaičiaus, kuris buvo panaudotas reikiamo sudėtingumo (nusakomo iš eilės einančių nulių skaičiumi hash'o pradžioje) naujojo bloko hash'ui gauti
        string  Difficulty_Target;
    public:
        
        Header() {
            this->Prev_Block_Hash = "";
            this->Timestamp = getCurrentTimestampISO8601();
            this->version = 1.0;
            this->Merkel_Root_Hash = "";
            this->Nonce = 0;
            this->Difficulty_Target = "";
        }
        Header(const std::string& prevHash, double ver, const std::string& merkelRoot, int nonce, const std::string& diffTarget) {
            this->Prev_Block_Hash = prevHash;
            this->Timestamp = getCurrentTimestampISO8601();
            this->version = ver;
            this->Merkel_Root_Hash = merkelRoot;
            this->Nonce = nonce;
            this->Difficulty_Target = diffTarget;
        }
        Header(const std::string& prevHash) {
            this->Prev_Block_Hash = prevHash;
            this->Timestamp = getCurrentTimestampISO8601();
            this->version = 1.0;
            this->Merkel_Root_Hash = "";
            this->Nonce = 0;
            this->Difficulty_Target = "";
        }

        string getPrev_Block_Hash() const { return Prev_Block_Hash; }
        void setPrev_Block_Hash(string _value) { this->Prev_Block_Hash = _value; }
        string getTimestamp() const { return Timestamp; }
        void setTimestamp(string _value) { this->Timestamp = _value; }
        double getVersion() const { return version; }
        void setVersion(double _value) { this->version = _value; }
        string getMerkel_Root_Hash() const { return Merkel_Root_Hash; }
        void setMerkel_Root_Hash (string const _value) { this->Merkel_Root_Hash = _value; }
        int getnonce() const { return Nonce; }
        void setNonce(int _value) { this->Nonce = _value; }

        string getdiffTarget() const { return Difficulty_Target; }
        void setdiffTarget(string _value) { this->Difficulty_Target = _value; }

        std::string getCurrentTimestampISO8601() const {
            auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::tm tm = *std::gmtime(&now);
            std::stringstream ss;
            ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
            return ss.str();
        }


    };
    class Body {
    private:
        vector<Transaction>body;
    public:
        Body() {
            this->body = {};
        }
        Body(vector<Transaction>g) {
            this->body = g;
        }

        inline std::vector<Transaction> getBody() const { return body; }  // get'eriai, inline
        inline void setBody(Transaction g) {
            body.push_back(g);
        };
        inline void setBody(vector<Transaction>g) {
            body = g;
        };
    };

    std::string calculateHash() const {
        std::stringstream ss;
        
        ss << BlockHeader.getPrev_Block_Hash() << BlockHeader.getVersion() << BlockHeader.getnonce() << BlockHeader.getTimestamp() << BlockHeader.getMerkel_Root_Hash() << BlockHeader.getdiffTarget();

        ss << BlockHeader.getMerkel_Root_Hash();
        
        string hash = ss.str();
        hash=customHash2(hash);
        return hash;
    };

    std::string calculateMerkleRoot(const std::vector<Transaction>& transactions) {
        if (transactions.empty()) {
            return "";
        }
        if (transactions.size() == 1) {
            return transactions[0].gethash();
        }

        std::vector<Transaction> nextLevel;
        for (size_t i = 0; i < transactions.size(); i += 2) {
            if (i + 1 < transactions.size()) {
                // Combine and hash pairs of transactions
                std::string combinedData = transactions[i].gethash() + transactions[i + 1].gethash();
                nextLevel.push_back(Transaction("", "", 0.0)); // Create a placeholder transaction
                nextLevel.back().sethash(customHash2(combinedData+ ""+ std::to_string(0.0)));
            }
            else {
                // For odd-sized lists, duplicate the last transaction
                nextLevel.push_back(transactions[i]);
            }
        }

        return calculateMerkleRoot(nextLevel);
    }

public:
    Header BlockHeader;
    Body BlockBody;
    string hash;

    Block() :BlockHeader(), BlockBody() {};
    Block(const std::string& prevHash, double ver, const std::string& merkelRoot, int nonce, const std::string& diffTarget, vector<Transaction>g) :BlockHeader(prevHash, ver, merkelRoot, nonce, diffTarget), BlockBody(g) {};
    Block(const std::string& prevHash, double ver, const std::string& merkelRoot, int nonce, const std::string& diffTarget) :BlockHeader(prevHash, ver, merkelRoot, nonce, diffTarget) {};
    Block(vector<Transaction>g) :BlockBody(g) {};

    Block(const std::string& prevHash) :BlockHeader(prevHash){
        BlockHeader.setTimestamp(BlockHeader.getCurrentTimestampISO8601());
        BlockHeader.setNonce(0);
        hash = mineBlock();
    }

    void addTransaction(const Transaction& transaction) {
        BlockBody.setBody(transaction);
        hash = mineBlock(); // Update the block's hash after adding a transaction.
    }

    void addTransactions(vector<Transaction> transactions) {
        BlockBody.setBody(transactions);
        hash = mineBlock(); // Update the block's hash after adding a transaction.
    }

    std::string mineBlock() {
        BlockHeader.setMerkel_Root_Hash(calculateMerkleRoot(BlockBody.getBody()));
        while (true) {
            BlockHeader.setTimestamp(BlockHeader.getCurrentTimestampISO8601());
            std::string hashAttempt = calculateHash();
            if (hashAttempt.substr(0, BlockHeader.getdiffTarget().size()) == BlockHeader.getdiffTarget()) {
                return hashAttempt;
            }
            BlockHeader.setNonce(BlockHeader.getnonce()+1);
            
        }
    }
    
    inline string getHash() const { return hash; }  // get'eriai, inline
    void setHash(string _value) { this->hash = _value; }

    
};







void UserGen(vector<User> &g) {

    User temp;
    const std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    using hrClock = std::chrono::high_resolution_clock;
    std::mt19937_64 mt(static_cast<long unsigned int>(hrClock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> dist(0, characters.size() - 1);
    std::uniform_int_distribution<int> dist2(100, 1000000);

    std::string randomString;
    randomString.reserve(15);

    for (int i = 0; i < 1000; i++) {
        for (int j = 0; j < 10; ++j) {
            randomString += characters[dist(mt)];
            temp.public_key += characters[dist(mt)];
        }
        temp.public_key = customHash2(temp.public_key);
        temp.setname(randomString);
        temp.seteur(dist2(mt));
        g.push_back(temp);
        randomString.clear();
        temp.public_key.clear();
    }
}

void TranGen(vector<Transaction>& g, vector<User> u) {
    Transaction temp;
    using hrClock = std::chrono::high_resolution_clock;
    std::mt19937_64 mt(static_cast<long unsigned int>(hrClock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> dist(0, 999);
    std::uniform_int_distribution<int> dist2(100, 1000000);
    string hash;
    int count = 0;
    for (int i = 0; i < 10000; i++) {
        temp.setsender(u[dist(mt)].getpublic_key());
        do {
            temp.setreceiver(u[dist(mt)].getpublic_key());
        } while (temp.getsender() == temp.getreceiver());
        temp.setsuma(dist2(mt));
        hash = temp.getsender() + temp.getreceiver()+ std::to_string(temp.getsuma());
        temp.sethash(customHash2(hash));
        g.push_back(temp);
        hash.clear();
    }
}

vector<Transaction> ChosenTransactions(vector<Transaction> g) {
    vector<Transaction> BlockTransactions;
    using hrClock = std::chrono::high_resolution_clock;
    std::mt19937_64 mt(static_cast<long unsigned int>(hrClock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> dist(0, 9999);
    for (int i = 0; i < 100; i++) {
        BlockTransactions.push_back(g[dist(mt)]);
    }
    return BlockTransactions;

}

class Blockchain {
private:
    std::vector<Block> chain;
    string Difficulty_Target="00";
    double Version = 1.0;

public:
    Blockchain() {
        // Create the genesis block.
        Block genesisBlock("0", Version, "MerkelRootHash1", 0, Difficulty_Target);
        Transaction tx1("Alice", "Bob", 5.0);
        genesisBlock.addTransaction(tx1);
        chain.push_back(genesisBlock);
    }

    void addBlock(const std::vector<Transaction>& transactions) {
        const Block& prevBlock = chain.back();
        Block newBlock(prevBlock.getHash(), prevBlock.BlockHeader.getVersion(), "MerkelRootHashN", 0, Difficulty_Target);
        newBlock.BlockBody.setBody(transactions);
        newBlock.setHash(newBlock.mineBlock());
        chain.push_back(newBlock);
    }

    const std::vector<Block>& getChain() const {
        return chain;
    }
};

void BlockInfo(Blockchain b) {
    // Display block information.

    const std::vector<Block>& chain = b.getChain();
    for (const Block& block : chain) {
        std::cout << "Previous Block Hash: " << block.BlockHeader.getPrev_Block_Hash() << std::endl;
        std::cout << "Merkel root hash: " << block.BlockHeader.getMerkel_Root_Hash()<< std::endl;
        std::cout << "Nonce: " << block.BlockHeader.getnonce() << std::endl;
        std::cout << "Version: " << block.BlockHeader.getVersion() << std::endl;
        std::cout << "Difficulty target: " << block.BlockHeader.getdiffTarget() << std::endl;
        std::cout << "Block Hash: " << block.getHash() << std::endl;
        std::cout << "Number of transactions in block: " << block.BlockBody.getBody().size() << std::endl;
        std::cout << "Timestamp: " << block.BlockHeader.getTimestamp() << std::endl;
        cout << endl;
        cout << "-----------------------------------------" << endl;
        cout << endl;
    }
}

void RemoveFromPool(Blockchain b, vector<Transaction>& t) {
    int count = 0;
    const std::vector<Block>& chain = b.getChain();
    
    for (const Block& block : chain) {
        cout << block.BlockBody.getBody().size() << endl;
        for (int i = 0; i < block.BlockBody.getBody().size(); i++) {
            for (int j = 0; j < t.size(); j++) {
                if (block.BlockBody.getBody()[i].gethash() == t[j].gethash()) { t.erase(t.begin() + j); count++; }
            }
        }
    }
    cout << count << endl;

}



int main()
{


    vector<User>u;
    vector<Transaction>t;
    vector<Transaction>ChosenT;
    UserGen(u);
    TranGen(t,u);
    ChosenT=ChosenTransactions(t);
    

    Blockchain blockchain;

    // Add transactions to a new block.
    blockchain.addBlock(ChosenT);
    // Access and display the blockchain.
    BlockInfo(blockchain);
    RemoveFromPool(blockchain, t);

    
    
























    /*for (auto& i : u) {
        cout << i.getname() << endl;
        cout<<i.getpublic_key()<<endl;
        cout<<i.geteur()<<endl;
    }
    */





    return 0;
}


