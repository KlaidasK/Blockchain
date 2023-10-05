# Blockchain

#v0.2

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
            sum = sum + static_cast<int>(input[ii]) * ^ k 
            sum % 31
            hash[i] = hash[i] ^ sum * 23
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
    input=G output=8615efe4dacd23c456bdcf9c3a55f3dc26256ff45a3da3d4d64d4facba45532c<br>
    input=& output=a619f168c6bd118c6ec9a908ee4d298c76f94128961de14c3e8979c8be0df94c<br>

    Du failai po 1001 elementų<br>
    output=eece1183620e85eba6fe99333abe0dab9eae81c3528e35cb761e69334a3edd2b<br>
    output=0cc74d3a90cf71da9447756ad8bf792a9c07ddfa202f619a0467a5aa48df09ea<br>

    Du failai po 1001 skiriantys tik vienu elementu<br>
    output=2210c14952a4498dd2884131027c79b5c260e1b9b274095d72b801c1e2acf9e5<br>
    output=c2604199b214699d52788161a24c792502b0a1a972e4a94dd2288151e2fcf9b5<br>

    Tuščias failas<br>
    output=Nera teksto<br>
2. 
   Output'ai visada yra vienodo dydžio, o to paties failo hash'as yra tas pats.
3. 
   |Eilutes|Laikas|
   | ------------- |------------- |
   |2|0,0005652 s|
   |4|0,0006151 s|
   |8|0,0009302 s|
   |16|0,0011859 s|
   |32|0,0015744 s|
   |64|0,0037779 s|

  
  ![alt text](https://github.com/KlaidasK/Blockchain/blob/0c10a6830f25a64686ca9b79f03a341823bb9885/Diag.png?raw=true)
