function [w, c, s ]= rbfn(X, t, k, h)
    p = length(X(:, 1))
    
    //calculate the centroids
    //centroids = neural_gas(1, k, X, 0.1, 10, 5, 50)
    centroids = fs_learning(2, k, X, 0.1, 10)
    //determine length of kernel Gaussian sigmas
    //distance from points to their centroid
    dists = zeros(k,1)
    //the number of element of clusters
    elems = zeros(k,1)
    for i = 1: p
        [cluster_index distance] = get_cluster(X(i, :) , centroids);
        dists(cluster_index) = dists(cluster_index) + distance;
        elems(cluster_index) = elems(cluster_index) + 1;
    end

    //average
    for i = 1: k
       sigmas(i) = h*dists(i)/elems(i);
    end

    //determine weights of output
    phis = ones(p, k + 1)
    for j = 1: k
        for i = 1: p
           e2 = (X(i, 1) - centroids(j, 1))^2 + (X(i, 2) - centroids(j, 2))^2;
           phis(i,j + 1) = exp(-e2/2/sigmas(j)^2);
        end
    end

    //phis(:, 1)  =
//    phis = phis';
disp(phis) 
    w = inv(phis'*phis)*phis'*t;
//    w = pinv(phis)*t;
    c = centroids;
    s = sigmas;
endfunction
