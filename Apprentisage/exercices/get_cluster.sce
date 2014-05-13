function [ind, d] = get_cluster(p, c)
    l = length(c(:,1))
    found = 0;
    min_dist = 2147483647
    for i = 1: l
        dist = sqrt( (p(1) - c(i, 1))^2 + (p(2) - c(i, 2))^2)
        if dist < min_dist then
            found = i;
            min_dist = dist;
        end
    end
    ind = found
    d = min_dist
endfunction
