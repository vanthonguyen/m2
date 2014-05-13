function re = regression_linaire(n, x, y)
    //regression lineare
    x2=x(:,n)
    x1=ones(length(x2),1)
    X=[x1, x2]
    W=inv(X'*X)*X'*y
    subplot(1,1,1)
    plot(x(:,n), y, '.')
    //xpolys(min(x2),W'*[1,min(x2)]',max(x2),W'*[1,max(x2)]')
    xpoly(x2', W'*X')
    re = W
endfunction
