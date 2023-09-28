# Blockchain

#v0.1

Hash funkcijos pseudokodas

    string customHash(string input){

    IF input is empty THEN
        RETURN "Nera teksto";
    END IF

    unsigned character hash[32]
    integer sum 
    integer k 
    
    FOR i=0 to 32 //Pirmas ciklas
        FOR i=0 to input.length //Antras ciklas
            sum = sum + static_cast<int>(input[ii]) * 23^k 
            hash[i] = hash[i] ^ sum
        END FOR
        k=k+1
    END FOR

    // Konvertavimas hash į hex 64 ilgio
    stringstream ss
    for i=0 to 32
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    END FOR
    return ss.str()
    }

#Eksperimentinis tyrimas ir rezultatų analizė

1.
    Du failai su po viena simbolį <br>
    input=G output=ad0fea482d8b62c4ad07ea402d8372dcad1fea582d9b62d4ad17ea502d9312ec<br>
    input=& output=a619f168c6bd118c6ec9a908ee4d298c76f94128961de14c3e8979c8be0df94c<br>

    Du failai po 1001 elementų<br>
    output=f97ebe8ba1726617a9c6aed3611ac6df990e1e3bc10266c709768ee3618a26ef<br>
    output=af599092b70518be572988824f95804e1f198092e7e5a89ea7a918021ff5b0ee<br>

    Du failai po 1001 skiriantys tik vienu elementu<br>
    output=a5f096d119e0a2c93d20ae4141c0ea3955b0c611898052e90d203e411180ba59<br>
    output=25e03621b970a2993df08e11e1d02a09954086e1a9b0f2398dd07e9171b09a29<br>

    Tuščias failas<br>
    output=Nera teksto<br>
2. 
   Output'ai visada yra vienodo dydžio, o to paties failo hash'as yra tas pats.
3. 
   |Eilutes|Laikas|
   | ------------- |------------- |
   |2|0.0003732 s|
   |4|0.0004559 s|
   |8|0.0006921 s|
   |16|0.0010487 s|
   |32|0.0038497 s|
   |64|0.0160818 s|

   ![alt text](https://github.com/KlaidasK/Blockchain/blob/main/Screenshot (1339).png?raw=true)
  
   
