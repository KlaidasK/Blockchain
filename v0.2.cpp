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
#include <locale>

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

    protected:
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
        virtual bool hasSufficientFunds(double amount) const {
            return eur >= amount;
        }

        ~User() { eur = 0; name.clear(); public_key.clear(); }
};

class Transaction{

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

        Transaction(string h, string s, string r, double e, vector<User>& g)
        {
            string hash;
            this->sender = s;
            this->receiver = r;
            this->suma = e;
            if (!CheckBalance(g)) {
                // Cancel the constructor if there are insufficient funds
                this->sender = "";
                this->receiver = "";
                this->suma = 0;
                return;
            }
            if (!CheckValidity()) {
                // Cancel the constructor if there are insufficient funds
                this->sender = "";
                this->receiver = "";
                this->suma = 0;
                return;
            }

            hash = sender + receiver + std::to_string(suma);
            UpdateUser(g);
            this->HashID = customHash2(hash);
        }

        Transaction(string s, string r, double e, vector<User>&g) {
            string hash;
            this->sender = s;
            this->receiver = r;
            this->suma = e;
            if (!CheckBalance(g)) {
                // Cancel the constructor if there are insufficient funds
                this->sender = "";
                this->receiver = "";
                this->suma = 0;
                return;
            }
            hash = sender + receiver + std::to_string(suma);
            UpdateUser(g);
            this->HashID = customHash2(hash);
        }

        bool CheckBalance(const std::vector<User>& g) {
            for (const auto& user : g) {
                if (sender == user.getpublic_key() && user.geteur() < suma) {
                    // Return false if there are insufficient funds
                    //std::cerr << "Error: Insufficient funds for sender." << std::endl;
                    return false;
                }
            }
            // Return true if the balance check is successful
            return true;
        }

        void UpdateUser(vector<User>& g) {
            for (auto &i : g) {
                if (receiver==i.getpublic_key()) { i.seteur(i.geteur() + suma); }
                if (sender == i.getpublic_key()) { i.seteur(i.geteur() - suma); }
            }
        }

        bool CheckValidity() {
            string hash;
            hash = sender + receiver + std::to_string(suma);
            if (HashID != customHash2(hash)) { cout << "Suklastota transakcija" << endl; return false; }
            return true;
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
            std::tm tm = *std::localtime(&now);
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
            for (auto i : g) {
                if(i.getsuma()!=0) body.push_back(i);
            }
        };
    };

    string calculateHash() const {
        std::stringstream ss;
        
        ss << BlockHeader.getPrev_Block_Hash() << BlockHeader.getVersion() << BlockHeader.getnonce() << BlockHeader.getTimestamp() << BlockHeader.getMerkel_Root_Hash() << BlockHeader.getdiffTarget();

        ss << BlockHeader.getMerkel_Root_Hash();
        
        string hash = ss.str();
        hash=customHash2(hash);
        return hash;
    };

    string calculateMerkleRoot(const std::vector<Transaction>& transactions) {
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
                vector <User> g;
                User g1("1", "1", 0.01);
                User g2("2", "2", 0);
                g.push_back(g1);
                g.push_back(g2);

                std::string combinedData = transactions[i].gethash() + transactions[i + 1].gethash();
                nextLevel.push_back(Transaction("1", "2", 0.0, g)); // Create a placeholder transaction
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

    void addTransaction(const Transaction transaction) {
        BlockBody.setBody(transaction);
        hash = mineBlock(); // Update the block's hash after adding a transaction.
    }

    void addTransactions(vector<Transaction> transactions) {
        BlockBody.setBody(transactions);
        hash = mineBlock(); // Update the block's hash after adding a transaction.
    }

    std::string mineBlock() {
        BlockHeader.setMerkel_Root_Hash(calculateMerkleRoot(BlockBody.getBody()));
        BlockHeader.setNonce(0);
        while (true) {
            BlockHeader.setTimestamp(BlockHeader.getCurrentTimestampISO8601());
            std::string hashAttempt = calculateHash();
            if (hashAttempt.substr(0, BlockHeader.getdiffTarget().size()) == BlockHeader.getdiffTarget()) {
                return hashAttempt;
            }
            BlockHeader.setNonce(BlockHeader.getnonce()+1);
        }
    }

    std::string LimitedMineBlock() {
        BlockHeader.setMerkel_Root_Hash(calculateMerkleRoot(BlockBody.getBody()));

        BlockHeader.setdiffTarget("0000");
        BlockHeader.setNonce(0);
        while (BlockHeader.getnonce() < 100000) {
                BlockHeader.setTimestamp(BlockHeader.getCurrentTimestampISO8601());
                std::string hashAttempt = calculateHash();

                if (hashAttempt.substr(0, BlockHeader.getdiffTarget().size()) == BlockHeader.getdiffTarget()) {
                    std::cout << "Hash'avimo bandymu skaicius: " << BlockHeader.getnonce() << std::endl;
                    return hashAttempt;
                }

                BlockHeader.setNonce(BlockHeader.getnonce() + 1);
            }

            throw std::runtime_error("Kasimas uztruko daugiau bandymu negu 100000.");
    }
    
    inline string getHash() const { return hash; }  // get'eriai, inline
    void setHash(string _value) { this->hash = _value; }

    
};

void UserGen(vector<User> &g, int sz) {

    User temp;
    const std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    using hrClock = std::chrono::high_resolution_clock;
    std::mt19937_64 mt(static_cast<long unsigned int>(hrClock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> dist(0, characters.size() - 1);
    std::uniform_int_distribution<int> dist2(100, 1000000);

    std::string randomString;
    randomString.reserve(15);

    for (int i = 0; i < sz; i++) {
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

void TranGen(vector<Transaction>& g, vector<User> u, int sz) {
    
    using hrClock = std::chrono::high_resolution_clock;
    std::mt19937_64 mt(static_cast<long unsigned int>(hrClock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> dist(0, u.size()-1);
    std::uniform_int_distribution<int> dist2(100, 1000000);
    string hash;
    int count = 0;
    for (int i = 0; i < sz; i++) {
        string sender="";
        string receiver="";
        double suma = 0;
        int index = dist(mt);
        sender = u[index].getpublic_key();
        
        suma = dist2(mt);
        do {
            suma = dist2(mt);
            index = dist(mt);
        } while (suma >= u[index].geteur());

        sender = u[index].getpublic_key();
        do{
           receiver = u[dist(mt)].getpublic_key();
        }while (sender == receiver);

        Transaction temp(sender, receiver, suma, u);

        hash = temp.getsender() + temp.getreceiver()+ std::to_string(temp.getsuma());
        temp.sethash(customHash2(hash));
        g.push_back(temp);
        hash.clear();
    }
}

vector<Transaction> ChosenTransactions(vector<Transaction> g, int sz) {
    if (g.size() < sz) return g;
    vector<Transaction> BlockTransactions;
    vector<Transaction> temp = g;
    using hrClock = std::chrono::high_resolution_clock;
    std::mt19937_64 mt(static_cast<long unsigned int>(hrClock::now().time_since_epoch().count()));
    
    for (int i = 0; i < sz; i++) {
        std::uniform_int_distribution<int> dist(0, temp.size() - 1);
        int index = dist(mt);
        BlockTransactions.push_back(temp[index]);
        temp.erase(temp.begin() + index);
    }
    return BlockTransactions;
}

class Blockchain {
private:
    std::vector<Block> chain;
    string Difficulty_Target="0000";
    double Version = 1.0;
    int maxTransactionsPerBlock = 100;  // Maximum transactions per block

public:
    Blockchain() {
        // Create the genesis block.
        vector<User>u;
        User u1("Alice", "wafjwoidjao", 100);
        User u2("Bob", "wafhaidhioajocowj", 100);
        u.push_back(u1);
        u.push_back(u2);
        Block genesisBlock("0", Version, "MerkelRootHash1", 0, Difficulty_Target);
        Transaction tx1("Alice", "Bob", 5.0, u);
        genesisBlock.addTransaction(tx1);
        chain.push_back(genesisBlock);
    }

    void addBlock(const std::vector<Transaction>& transactions) {
        if (transactions.size() <= maxTransactionsPerBlock) {
            const Block& prevBlock = chain.back();
            Block newBlock(prevBlock.getHash(), prevBlock.BlockHeader.getVersion(), "MerkelRootHashN", 0, Difficulty_Target);
            newBlock.BlockBody.setBody(transactions);
            newBlock.setHash(newBlock.mineBlock());
            chain.push_back(newBlock);
        }
        else { cout << "Transakciju kiekis negali būti didesnis uz 100" << endl; }
    }

    void addTransactionToBlock(int blockIndex, const Transaction& transaction) {
        
        if (blockIndex >= 0 && blockIndex < chain.size()) {
            if (chain[blockIndex].BlockBody.getBody().size() < maxTransactionsPerBlock) {
                chain[blockIndex].addTransaction(transaction);
                chain[blockIndex].setHash(chain[blockIndex].mineBlock());
            }
            else {
                std::cout << "Transakciju kiekis negali buti didesnis uz 100" << std::endl;
            }
        }
        else {
            std::cout << "Neteisingas bloko indexas." << std::endl;
        }
    }

    void addTransactionsToBlock(int blockIndex, const vector<Transaction> t) {
        if (chain[blockIndex].BlockBody.getBody().size() + t.size() < maxTransactionsPerBlock) {
            for (auto i : t) {
                if (blockIndex >= 0 && blockIndex < chain.size()) {
                    chain[blockIndex].addTransaction(i);
                    chain[blockIndex].setHash(chain[blockIndex].mineBlock());
                }
                else {
                    std::cout << "Neteisingas bloko indexas." << std::endl;
                }
            }
        }
        else { cout << "Blokas pilnas. Pasirinkite kitą." << endl; }
    }

    // Function to return a specific block by index
    const Block& getBlockByIndex(int blockIndex) const {
        if (blockIndex >= 0 && blockIndex < chain.size()) {
            return chain[blockIndex];
        }
        else {
            // Handle the case where the block index is invalid
            // For now, returning a default-constructed block; you may want to throw an exception or handle differently
            std::cout << "Invalid block index. Returning a default-constructed block." << std::endl;
            return chain.back();
        }
    }

    const Block& getLastBlock() const {
            return chain.back();
    }

    const std::vector<Block>& getChain() const {
        return chain;
    }
};

void BlockInfo(Blockchain b) {
    // Display block information.

    Block block = b.getLastBlock();

        cout << endl;
        cout << "-----------------------------------------" << endl;
        cout << endl;
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

void BlockInfoByIndex(Blockchain b, int index) {
    // Display block information.

    Block block = b.getBlockByIndex(index);

    cout << endl;
    cout << "-----------------------------------------" << endl;
    cout << endl;
    std::cout << "Previous Block Hash: " << block.BlockHeader.getPrev_Block_Hash() << std::endl;
    std::cout << "Merkel root hash: " << block.BlockHeader.getMerkel_Root_Hash() << std::endl;
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

void TransactionInfo(vector<Transaction>t) {
    cout << endl;
    for (auto i : t) {
        cout << "Transakcijos Id: " << i.gethash() << std::setw(64);
        cout << "Transakcijos suma: " << i.getsuma() << endl;
    }
    cout << endl;



}

void RemoveFromPool(Blockchain b, vector<Transaction>& t) {
    int count = 0;
    Block block = b.getLastBlock();
    if (block.getHash() == b.getBlockByIndex(0).getHash()) return;

    for (auto it = t.begin(); it != t.end();) {
        bool erased = false;
        for (const auto& blockTx : block.BlockBody.getBody()) {
            if (blockTx.gethash() == it->gethash()) {
                it = t.erase(it);
                erased = true;
                count++;
                break; // Exit the inner loop after erasing
            }
        }

        // Increment the iterator only if no erasing occurred
        if (!erased) {
            ++it;
        }
    }

        cout << "Pasalintu transakciju kiekis: " << count << endl;
}


void saveTransactions(vector<Transaction> t, string name) {

    std::ofstream fr(name);

    for (auto i : t) {
        fr << i.gethash() << " " << i.getsender() << " " << i.getreceiver() << " " << i.getsuma() << endl;

    }
}

void readTransactions(vector<Transaction>& t, string name) {
    std::vector<std::string> splited;
    std::string word, line;
    int max = 0;
    double sk = 0.0; // Iš viso žodžių faile
    int stul = 0; //stulpelių sk.
    Transaction temp;
    std::ifstream file(name);

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream ss(line);

            string h, s, r = "";
            double p = 0;
            ss >> h >> s >> r >> p;
            temp.sethash(h);
            temp.setsender(s);
            temp.setreceiver(r);
            temp.setsuma(p);
            t.push_back(temp);
        }
        file.close();
    }
}

void UserInfo(vector<User> u) {


    cout << endl;
    for (auto i : u) {
        cout << "Naudotojo vardas: " << i.getname() << std::setw(64);
        cout << "Naudotojo balanasas: " << i.geteur() << endl;
    }
    cout << endl;
}

void TransactionInfo(vector<Transaction> t, int index) {
    if (index >= 0 && index < t.size()) {
        cout<<"Transakcijos ID: "<<t[index].gethash()<< endl;
        cout<<"Siuntejas: "<< t[index].getsender() << endl;
        cout<<"Gavejas: "<< t[index].getreceiver() << endl;
        cout<<"Suma:"<< t[index].getsuma() << endl;
    }


}

void Mining_Test(Blockchain &b, vector<User>u, vector<Transaction> t) {
    vector<Transaction>ChosenT;
    for (int i = 0; i < 5; i++) {
        ChosenT = ChosenTransactions(t, 100);
        b.addBlock(ChosenT);
        cout << "Sukurtas Blokas" << endl;
    }
    cout << "Blokai sukurti" << endl;
    std::vector<Block> blocks = b.getChain();

    
    std::random_device rd;
    std::mt19937 g(rd());
    int count = 0;

    while (!blocks.empty()) {
        // Shuffle the blocks to choose a random order
 
        std::shuffle(blocks.begin(), blocks.end(), g);

        // Randomly choose a block
        Block& block = blocks.back();
        blocks.pop_back();

        try {
            count++;
            string minedHash = block.LimitedMineBlock();
            
            // Print the results
            std::cout << "Iskasto bloko hash'as: " << minedHash << std::endl;
            // Successfully mined a block, exit the loop
            cout << "Bandymai: " << count << endl;
            break;
        }
        catch (const std::runtime_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

}



int main()
{
    int t_sz = 10000;
    int u_sz = 1000;
    cout << "Iveskite naudotoju kieki: " << endl;
    cin >> u_sz;
    cout << "Iveskite transakciju kieki: " << endl;
    cin >> t_sz;
    vector<User>u;
    vector<Transaction>t;
    vector<Transaction>ChosenT;
    UserGen(u,u_sz);
    TranGen(t,u,t_sz);
    //TransactionInfo(t, 47);



    Blockchain blockchain;
    //UserInfo(u);
    Mining_Test(blockchain,u,t);
    //BlockInfoByIndex(blockchain,2);
    
    
    /*while (t.size() >= 100) {
        cout << endl;
        
        ChosenT = ChosenTransactions(t, 100);
        
        blockchain.addBlock(ChosenT);
        BlockInfo(blockchain);
        TransactionInfo(ChosenT);
        RemoveFromPool(blockchain, t);
        cout << "Transactions in block removed from Transaction pool" << endl;
        cout << "Transakciju kiekis: " << t.size() << endl << endl;
        ChosenT.clear();
   } 
   */
    //UserInfo(u);
    

   


    return 0;
}


