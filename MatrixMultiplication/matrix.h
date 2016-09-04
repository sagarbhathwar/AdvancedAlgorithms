#pragma once

#include <vector>
#include <cassert>
#include <iostream>
#include <iterator>

template <typename T>
class Matrix
{
private:
	std::vector<std::vector<T>> matrix_;
	int n_rows_;
	int n_cols_;

	static Matrix<T> recursive_matrix_multiplication_(Matrix<T>& A, int na_row_start, int na_row_end, int na_col_start, int na_col_end,
		Matrix<T>& B, int nb_row_start, int nb_row_end, int nb_col_start, int nb_col_end);

public:
	Matrix(int n_rows, int n_cols);

	template <typename T_>
	friend std::istream& operator>>(std::istream& is, Matrix<T_>& matrix);

	template <typename T_>
	friend std::ostream& operator<<(std::ostream& os, const Matrix<T_>& matrix);

	template <typename T_>
	friend Matrix<T> strassen_multiplication(Matrix<T_>& A, Matrix<T_>& B);

	static Matrix<T> recursive_matrix_multiplication(Matrix<T>& A, Matrix<T>& B);

	Matrix<T> operator*(const Matrix<T>& other);

	Matrix<T> operator+(const Matrix<T>& other);

	std::vector<T>& operator[](int i);

	T& operator()(int i, int j);

	const T& operator()(int i, int j) const;

	bool operator==(const Matrix<T>& other);
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
Matrix<T> Matrix<T>::operator*(const Matrix<T>& other)
{
assert(this->n_cols_ == other.n_rows_);
	Matrix<T> result(this->n_rows_, other.n_cols_);
	for (int i = 0; i < this->n_rows_; ++i)
	{
		for (int j = 0; j < other.n_cols_; ++j)
		{
			T total = 0;
			for (int k = 0; k < this->n_cols_; ++k)
			{
				total += this->matrix_[i][k] * other.matrix_[k][j];
			}
			result.matrix_[i][j] = total;
		}
	}

	return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& other)
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
Matrix<T> strassen_multiplication(const Matrix<T>& A, const Matrix<T>& B)
{

}

template <typename T>
Matrix<T> Matrix<T>::recursive_matrix_multiplication_(Matrix<T>& A, int na_row_start, int na_row_end, int na_col_start, int na_col_end,
	Matrix<T>& B, int nb_row_start, int nb_row_end, int nb_col_start, int nb_col_end)
{
    /*
	std::cout << na_row_start << " " << na_row_end << " "
		<< na_col_start << " " << na_col_end << " " <<
		nb_row_start << " " << nb_row_end << " "
		<< nb_col_start << " " << nb_col_end << std::endl;
    */
    int n = na_row_end - na_row_start;
	Matrix<T> C(n, n);
    if (n == 1) C(0, 0) = A(na_row_start, na_col_start) * B(nb_row_start, nb_col_start);

	//Partition A, B, C
	else
	{
        Matrix<T> C00(recursive_matrix_multiplication_(A, na_row_start, (na_row_end + na_row_start) / 2, na_col_start, (na_col_end + na_col_start) / 2, //A11
            B, nb_row_start, (nb_row_end + nb_row_start) / 2, nb_col_start, (nb_col_end + nb_col_start) / 2) //B11
			+
            recursive_matrix_multiplication_(A, na_row_start, (na_row_end + na_row_start) / 2, (na_col_end + na_col_start) / 2, na_col_end, //A12
                B, (nb_row_end + nb_row_start) / 2, nb_row_end, nb_col_start, (nb_col_end + nb_col_start) / 2)); //B21

        Matrix<T> C01(recursive_matrix_multiplication_(A, na_row_start, (na_row_end + na_row_start) / 2, na_col_start, (na_col_end + na_col_start) / 2, //A11
            B, nb_row_start, (nb_row_end + nb_row_start) / 2, (nb_col_end + nb_col_start) / 2, nb_col_end) // B12
			+
            recursive_matrix_multiplication_(A, na_row_start, (na_row_end + na_row_start) / 2, (na_col_end + na_col_start) / 2, na_col_end, //A12
                B, (nb_row_end + nb_row_start) / 2, nb_row_end, (nb_col_end + nb_col_start) / 2, nb_col_end)); //B22

        Matrix<T> C10(recursive_matrix_multiplication_(A, (na_row_end + na_row_start) / 2, na_row_end, na_col_start, (na_col_end + na_col_start) / 2, //A21
            B, nb_row_start, (nb_row_end + nb_row_start) / 2, nb_col_start, (nb_col_end + nb_col_start) / 2) //B11
			+
            recursive_matrix_multiplication_(A, (na_row_end + na_row_start) / 2, na_row_end, (na_col_end + na_col_start) / 2, na_col_end, //A22
                B, (nb_row_end + nb_row_start) / 2, nb_row_end, nb_col_start, (nb_col_end + nb_col_start) / 2)); //B21

        Matrix<T> C11(recursive_matrix_multiplication_(A, (na_row_end + na_row_start) / 2, na_row_end, na_col_start, (na_col_end + na_col_start) / 2, //A21
            B, nb_col_start , (nb_row_end + nb_row_start) / 2, (nb_col_end + nb_col_start) / 2, nb_col_end) //B12
			+
            recursive_matrix_multiplication_(A, (na_row_end + na_row_start) / 2, na_row_end, (na_col_end + na_col_start) / 2, na_col_end, //A22
                B, (nb_row_end + nb_row_start) / 2, nb_row_end, (nb_col_end + nb_col_start) / 2, nb_col_end)); //B22

        std::cout << "Matrix is: " << C00 << " " << C01 << " " << C10 << " " << C11 << std::endl;

		for (int i = 0; i < n / 2; ++i)
			for (int j = 0; j < n / 2; ++j)
				C(i, j) = C00(i, j);

		for (int i = 0; i < n / 2; ++i)
			for (int j = n / 2; j < n; ++j)
				C(i, j) = C01(i, n / 2 - j);

		for (int i = n / 2; i < n; ++i)
			for (int j = 0; j < n / 2; ++j)
				C(i, j) = C10(n / 2 - i, j);

		for (int i = n / 2; i < n; ++i)
			for (int j = n / 2; j < n; ++j)
				C(i, j) = C11(n / 2 - i, n / 2 - j);
	}

	return C;
}

template <typename T>
Matrix<T> Matrix<T>::recursive_matrix_multiplication(Matrix<T>& A, Matrix<T>& B)
{
	return recursive_matrix_multiplication_(A, 0, A.n_rows_, 0, A.n_cols_, B, 0, B.n_rows_, 0, B.n_cols_);
}

template<typename T>
std::vector<T>& Matrix<T>::operator[](int i)
{
	return this->matrix_[i];
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
bool Matrix<T>::operator==(const Matrix<T>& other)
{
	if (this->n_rows_ != other.n_rows_ || this->n_cols_ != other.n_rows_)
	for (int i = 0; i < this->n_rows_; ++i)
	{
		for (int j = 0; j < this->n_cols_; ++j)
		{
			if (this->matrix_[i][j] != other(i, j)) return false;
		}
	}

	return true;
}
