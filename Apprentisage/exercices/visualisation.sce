[w c s] = rbfn(X_train, T_train, 30, 1.5)
x1 = -3:0.1:3;
x2 = -3:0.1:3;

[X1, X2] = meshgrid(x1, x2)
for i = 1:size(X1, 1)
    for j = 1:size(X1, 2)
        Y(i,j) = rbf([X1(i, j), X2(i,j)], w, c, s);
    end    
end
surf(X1,X2,Y)

//test
for i = 1: size(X_train, 1)
    R1(i) = rbf(X_train(i,:), w, c, s);
end
//plot(R1, T_train, '.')
