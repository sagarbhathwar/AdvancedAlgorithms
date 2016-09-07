//Compiled with "g++ -std=c++14 -O3 <filename.cpp>"
#include <vector>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <chrono>

#define BLOCK_SIZE 8

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

    static Matrix<T> strassen_multiplication(const Matrix<T>& A, const Matrix<T>& B);

    Matrix<T> transpose() const;

    Matrix<T> operator*(const Matrix<T>& other) const;

    Matrix<T> operator+(const Matrix<T>& other) const;

    Matrix<T> operator-(const Matrix<T>& other) const;

    std::vector<T>& operator[](int i);

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
Matrix<T> Matrix<T>::operator+(const Matrix<T>& other) const
{
assert(this->n_cols_ == other.n_cols_ && this->n_rows_ == other.n_rows_);
	Matrix<T> result(this->n_rows_, this->n_cols_);
	for (int i = 0; i < this->n_rows_; ++i)
	{
		for (int j = 0; j < this->n_cols_; ++j)
		{
			result[i][j] = this->matrix_[i][j] + other.matrix_[i][j];
		}
	}
	return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& other) const
{
assert(this->n_cols_ == other.n_cols_ && this->n_rows_ == other.n_rows_);
    Matrix<T> result(this->n_rows_, this->n_cols_);
    for (int i = 0; i < this->n_rows_; ++i)
    {
        for (int j = 0; j < this->n_cols_; ++j)
        {
            result[i][j] = this->matrix_[i][j] - other.matrix_[i][j];
        }
    }
    return result;
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

template<typename T>
std::vector<T>& Matrix<T>::operator[](int i)
{
    return this->matrix_[i];
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

template <typename T>
Matrix<T> Matrix<T>::strassen_multiplication(const Matrix<T>& A, const Matrix<T>& B)
{
    int n = A.n_rows_;

    if(n <= BLOCK_SIZE)
    {
        return A * B;
    }

    else
    {
        Matrix<T> a00(n / 2, n / 2), b00(n / 2, n / 2), a01(n / 2, n / 2), b01(n / 2, n / 2),
                a10(n / 2, n / 2), b10(n / 2, n / 2), a11(n / 2, n / 2), b11(n / 2, n / 2);

        for(int i = 0; i < n / 2; ++i)
        {

            a00.matrix_[i] = std::vector<T>(A.matrix_[i].begin(), A.matrix_[i].begin() + n / 2);
            b00.matrix_[i] = std::vector<T>(B.matrix_[i].begin(), B.matrix_[i].begin() + n / 2);
            a01.matrix_[i] = std::vector<T>(A.matrix_[i].begin() + n / 2, A.matrix_[i].end());
            b01.matrix_[i] = std::vector<T>(B.matrix_[i].begin() + n / 2, B.matrix_[i].end());
        }

        for(int i = n / 2; i < n; ++i)
        {
            a10.matrix_[i - n / 2] = std::vector<T>(A.matrix_[i].begin(), A.matrix_[i].begin() + n / 2);
            b10.matrix_[i - n / 2] = std::vector<T>(B.matrix_[i].begin(), B.matrix_[i].begin() + n / 2);
            a11.matrix_[i - n / 2] = std::vector<T>(A.matrix_[i].begin() + n / 2, A.matrix_[i].end());
            b11.matrix_[i - n / 2] = std::vector<T>(B.matrix_[i].begin() + n / 2, B.matrix_[i].end());
        }

        Matrix<T> m1 = strassen_multiplication(a00 + a11, b00 + b11);
        Matrix<T> m2 = strassen_multiplication(a10 + a11, b00);
        Matrix<T> m3 = strassen_multiplication(a00, b01 - b11);
        Matrix<T> m4 = strassen_multiplication(a11, b10 - b00);
        Matrix<T> m5 = strassen_multiplication(a00 + a01, b11);
        Matrix<T> m6 = strassen_multiplication(a10 - a00, b00 + b01);
        Matrix<T> m7 = strassen_multiplication(a01 - a11, b10 + b11);

        Matrix<T> C00 = m1 + m4 - m5 + m7;
        Matrix<T> C01 = m3 + m5;
        Matrix<T> C10 = m2 + m4;
        Matrix<T> C11 = m1 + m3 - m2 + m6;

        Matrix<T> C(n, n);

        for (int i = 0; i < n / 2; ++i)
            for (int j = 0; j < n / 2; ++j)
                C(i, j) = C00(i, j);

        for (int i = 0; i < n / 2; ++i)
            for (int j = n / 2; j < n; ++j)
                C(i, j) = C01(i, j - n / 2);

        for (int i = n / 2; i < n; ++i)
            for (int j = 0; j < n / 2; ++j)
                C(i, j) = C10(i - n / 2, j);

        for (int i = n / 2; i < n; ++i)
            for (int j = n / 2; j < n; ++j)
                C(i, j) = C11(i - n / 2, j - n / 2);

        return C;
    }
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
    auto C1 = Matrix<float>::strassen_multiplication(A, B);
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << double((t2 - t1).count()) / (1000000000) << std::endl;
}
