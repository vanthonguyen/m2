function show_quantization(X, X_c)
// Visualise a vector quantization.
//
//   visualise(X, X_c)
//
// Inputs:
//   - X: instances (p x 2 vector)
//   - X_c: centroids (q x 2 vector)
//
// A figure is created and the p instances are shown as dots,
// whereas the q centroids are shown as diamonds.
//
// Author: Benoit Frenay (2012) - benoit.frenay@uclouvain.be
// Version: 12-10-12-11:25


//// check arguments
if ~exists('X', 'l')
    error('input X is missing');
end
if size(X, 2) ~= 2
    error('incorrect size for argument X');
end

if ~exists('X_c', 'l')
    error('input X_c is missing');
end
if size(X, 2) ~= 2
    error('incorrect size for argument X_c');
end


//// show vector quantization
figure;

plot(X(:, 1), X(:, 2), 'k.', 'MarkerSize', 5);
plot(X_c(:, 1), X_c(:, 2), 'd', 'MarkerSize', 10, 'MarkerFaceColor', 'm', 'MarkerEdgeColor', 'k');

xlabel('X_1');
ylabel('X_2');
title(msprintf('vector quantization (%d samples - %d centroids)', size(X, 1), size(X_c, 1)));

endfunction
