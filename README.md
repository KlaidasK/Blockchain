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
