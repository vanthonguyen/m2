function visualise(X, T)
// Visualise the values of a function with bidimensional inputs.
//
//   visualise(X, T)
//
// Inputs:
//   - X: instances (n x 2 matrix)
//   - T: function values (n x 1 vector)
//
// visualise does not create a new figure, which means that
// the visualisation will be shown in the current (sub)plot.
//
// Author: Benoit Frenay (2011) - benoit.frenay@uclouvain.be
// Version: 26-10-12-07:38


// check arguments
if ~exists('X', 'l')
    error('input X is missing');
end
if size(X, 2) ~= 2
    error('incorrect size for argument X');
end
if ~exists('T', 'l')
    error('input T is missing');
end
if size(T, 2) ~= 1
    error('incorrect size for argument T');
end
if size(X, 1) ~= size(T, 1)
    error('sizes of arguments X and T are different');
end


// compute Delaunay triangularisation
//tri = delaunay(X(:, 1), X(:, 2));


// visualise triangular surface plot
//trisurf(tri, X(:, 1), X(:, 2), T);
param3d(X(:, 1), X(:, 2), T);
p = gce();
p.mark_mode = 'on';
p.line_mode = 'off';
xlabel('X_1');
ylabel('X_2');
zlabel('T');
endfunction
