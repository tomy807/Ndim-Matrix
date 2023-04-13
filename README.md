# Ndim-Matrix
N-dimensional Matrix like Eigen

# matrix operations
## Dynamic And Fixed Matrix
- Dynamic Matrix : MatrixX{i,f,d} \
Resizable matrix
- Fixed Matrix : Matrix{2,3,4}{i,f,d}\
2x2, 3x3, 4x4 Size of Matrix
- Semi Dynamic Matrix : Matrix{N}{X}{i,f,d}, Matrix{X}{N}{i,f,d}\
Can Resize Row or Column
#
- std::cout << Matrix
- std::ostream operator<< Matrix
- operator +,*,- (if Size is Same, Dynamic <-> Fixed)
- operator= (if Size is Same, Dynamic <-> Fixed)
- row, col 
- transpose
- add, sub, mul
- Matrix multiplication
- block
- reduction scalar assignment, addition, subtraction, multiplication, division

# ToDoList
- reshape
- Inner product
- Outer product
- LU decomposition + Gaussian elimination
- QR decomposition
- Deteminant, Trace
- Norm
- Vector normalization
- Singular Value Decomposition (able to low rank approx SVD)
- Matrix Inverse, Pseudoinverse
- Eigenvalues + Eigenvectors