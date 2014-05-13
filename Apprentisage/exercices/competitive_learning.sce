function codebook = competitive_learning(codebook_type, q, X, a, b)
    min_x1 = min(X(:, 1))
    max_x1 = max(X(:, 1))
    
    min_x2 = min(X(:, 2))
    max_x2 = max(X(:, 2))
    p = length(X(:, 1))
    cb = zeros(q, 2)
    if codebook_type == 1 then
        for i = 1: q
            cb(i, 1) = grand(1, 1, 'unf', min_x1, max_x1)
            cb(i, 2) = grand(1, 1, 'unf', min_x2, max_x2)
        end
    else                
        for i = 1: q
            index = grand(1, 1, 'unf', 1, p)
            cb(i, :) = X(index, :)
        end        
    end
    delta = 2147483647
    while delta > 0.00001
        //store current codebook
        cb_prev = cb
        //loop through data
        for i = 1: p
            //max value of integer            
            min_distance = 2147483647
            for j = 1: q
                d_ij = sqrt( (X(i, 1) - cb(j, 1))^2 + (X(i, 2) - cb(j, 2))^2)
                if d_ij < min_distance then
                    min_distance = d_ij
                    winner = j
                end
            end
            
            //adapt winner
            cb(winner, :) = cb(winner, :) + a*( X(i, :) - cb(winner, :))    
        end
        
        //update alpha
        a = a*b/(a + b)
        //check for converge
        delta = 0
        for j = 1: q
            delta = delta + (cb(j, 1) - cb_prev(j, 1))^2 + (cb(j, 2) - cb_prev(j, 2))^2
        end
    end
    show_quantization(X, cb)
    codebook = cb
endfunction
