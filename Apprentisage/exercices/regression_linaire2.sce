function re = regression_linaire2(n, x, y)
    //regression lineare
    x2=x(:,n)
    x1=ones(length(x2),1)
    X=[x1, x2]
    W=inv(X'*X)*X'*y
    //two variables
    for i = 1:10
        err = y - X*W
        rel2(i) = corr(x(:,i), err, 1)/sqrt(variance(err))/sqrt(variance(x(:,i)))
    end
    [V I] = max(rel2)
    x3=x(:,I)
    x1=ones(length(x2),1)
    
    X2=[ x1 x2 x3]
    W2=inv(X2'*X2)*X2'*y
    re = W2
endfunction
