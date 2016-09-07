//Compiled with "g++ -std=c++14 -O3 <filename.cpp>"
#include <vector>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <chrono>

template <typename T>
class Matrix
{
private:
	std::vector<std::vector<T>> matrix_;
	int n_rows_;
    int n_cols_;

public:
	Matrix(int n_rows, int n_cols);

    Matrix() {}

	template <typename T_>
	friend std::istream& operator>>(std::istream& is, Matrix<T_>& matrix);

	template <typename T_>
    friend std::ostream& operator<<(std::ostream& os, const Matrix<T_>& matrix);

    Matrix<T> transpose() const;

    Matrix<T> operator*(const Matrix<T>& other) const;

	T& operator()(int i, int j);

    const T& operator()(int i, int j) const;
};

template<typename T>
Matrix<T>::Matrix(int n_rows, int n_cols)
{
	matrix_ = std::vector<std::vector<T>>(n_rows, std::vector<T>(n_cols));
	n_rows_ = n_rows;
	n_cols_ = n_cols;
}

template <typename T>
std::istream& operator>>(std::istream& is, Matrix<T>& matrix)
{
	for (int i = 0; i < matrix.matrix_.size(); ++i)
	{
		for (int j = 0; j < matrix.matrix_[i].size(); ++j)
		{
			is >> matrix.matrix_[i][j];
		}
	}
	return is;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& matrix)
{
	for (int i = 0; i < matrix.matrix_.size(); ++i)
	{
		for (int j = 0; j < matrix.matrix_[i].size(); ++j)
		{
			os << matrix.matrix_[i][j] << ' ';
		}
		os << std::endl;
	}
	return os;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& other) const
{
assert(this->n_cols_ == other.n_rows_);
    Matrix<T> result(this->n_rows_, other.n_cols_);
    Matrix<T> transpose_of_B = other.transpose();
    for (int i = 0; i < this->n_rows_; ++i)
    {
        for (int j = 0; j < other.n_cols_; ++j)
        {
            T total = 0;
            for (int k = 0; k < this->n_cols_; ++k)
            {
                total += this->matrix_[i][k] * transpose_of_B.matrix_[j][k];
            }
            result.matrix_[i][j] = total;
        }
    }

    return result;
}

template <typename T>
T& Matrix<T>::operator()(int i, int j)
{
    return matrix_[i][j];
}

template <typename T>
const T& Matrix<T>::operator()(int i, int j) const
{
    return matrix_[i][j];
}

template <typename T>
Matrix<T> Matrix<T>::transpose() const
{
    Matrix<T> transpose_matrix(n_cols_, n_rows_);
    for(int i = 0; i < n_rows_; ++i)
    {
        for(int j = 0; j < n_cols_; ++j)
        {
            transpose_matrix.matrix_[i][j] = this->matrix_[j][i];
        }
    }

    return transpose_matrix;
}

int main()
{
    int n;
    std::cin >> n;

    Matrix<float> A(n, n), B(n , n);

    for(int i = 0; i < n; ++i)
    {
        for(int j = 0; j < n; ++j)
        {
            std::cin >> A(i, j);
        }
    }

    for(int i = 0; i < n; ++i)
    {
        for(int j = 0; j < n; ++j)
        {
            std::cin >> B(i, j);
        }
    }

    std::cout << std::fixed << std::setprecision(6);

    auto t1 = std::chrono::high_resolution_clock::now();
    auto C1 = A * B;
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << double((t2 - t1).count()) / (1000000000) << std::endl;
}