function codebook = neural_gas(codebook_type, q, X, a, b, l, g)
    min_x1 = min(X(:, 1))
    max_x1 = max(X(:, 1))
    
    min_x2 = min(X(:, 2))
    max_x2 = max(X(:, 2))
    p = length(X(:, 1))
    cb = zeros(q, 2)
    u = ones(q,1)
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

    epoque = 0
    delta = 2147483647
    while delta > 0.000001
        epoque = epoque + 1;
        //store current codebook
        cb_prev = cb
        //loop through data
        for i = 1: p
            for j = 1: q
                //distance from point i to centroid j
                d(j) = sqrt( (X(i, 1) - cb(j, 1))^2 + (X(i, 2) - cb(j, 2))^2)
            end          
            [V h] = gsort(d, 'r', 'i')
            //adapt the centroids
            for k = 1:q
                cb(h(k), :) = cb(h(k), :) + a*exp(-(k - 1)/l) *( X(i, :) - cb(h(k), :))
            end
        end
                
        //update alpha and lamda
        a = a*b/(a + b)
        l = l*g/(l + g)
        
        //check for converge
        delta = 0
        for j = 1:q
            delta = delta + (cb(j, 1) - cb_prev(j, 1))^2 + (cb(j, 2) - cb_prev(j, 2))^2
        end
    end
//    show_quantization(X, cb)
    codebook = cb;
//    disp("Converge in ")
//    disp(epoque)    
endfunction
