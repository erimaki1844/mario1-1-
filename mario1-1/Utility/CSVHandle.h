#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

template <typename T>
class CSVFile {
public:
    string filepath;
    bool isHeader;
    bool isIndex;
    char delim;
    // �w�b�_�[���ۊ�
    vector<string> header;
    // �C���f�b�N�X���ۊ�
    vector<string> index;
    // �v�f�ۊ�
    vector<vector<T>> cell;

    // �R���X�g���N�^�֐�
    CSVFile() {
        isHeader = false;
        isIndex = false;
        delim = ',';
    }
    T cast_cell(string);
    void csv_read(string, bool, bool, char);
    void csv_write(string, char);
    void csv_show();
};

// cell �̌^�ɉ������L���X�g�����̒�`
int CSVFile<int>::cast_cell(string str) {
    return stoi(str);
}
double CSVFile<double>::cast_cell(string str) {
    return stod(str);
}
string CSVFile<string>::cast_cell(string str) {
    return str;
}

// CSV�t�@�C����ǂ݂���
template <typename T>
void CSVFile<T>::csv_read(string filepath, bool isHeader, bool isIndex, char delim) {
    this->filepath = filepath;
    this->isHeader = isHeader;
    this->isIndex = isIndex;
    this->delim = delim;

    string str_buf;
    string str_comma_buf;

    // �ǂݍ���csv�t�@�C�����J��(ifstream�̃R���X�g���N�^�ŊJ��)
    ifstream ifs_csv_file(filepath);

    // getline�֐���1�s���ǂݍ���(�ǂݍ��񂾓��e�� str_buf �Ɋi�[)
    for (int i = 0; getline(ifs_csv_file, str_buf); i++) {
        cell.push_back(vector<T>());

        // delim ��؂育�ƂɃf�[�^��ǂݍ��ނ��߂� istringstream �ɂ���
        istringstream i_stream(str_buf);

        for (int j = 0; getline(i_stream, str_comma_buf, delim); j++) {
            if (isHeader && isIndex) {
                if (i == 0 && j == 0) continue;
                if (i == 0 && j != 0) header.push_back(str_comma_buf);
                if (i != 0 && j == 0) index.push_back(str_comma_buf);
                if (i != 0 && j != 0) cell.at(i - 1).push_back(cast_cell(str_comma_buf));
            }
            else if (isHeader) {
                if (i == 0) header.push_back(str_comma_buf);
                if (i != 0 && j == 0) index.push_back(string());
                if (i != 0) cell.at(i - 1).push_back(cast_cell(str_comma_buf));
            }
            else if (isIndex) {
                if (i == 0 && j != 0) header.push_back(string());
                if (j == 0) index.push_back(str_comma_buf);
                if (j != 0) cell.at(i).push_back(cast_cell(str_comma_buf));
            }
            else {
                if (i == 0) header.push_back(string());
                if (j == 0) index.push_back(string());
                cell.at(i).push_back(cast_cell(str_comma_buf));
            }
        }
    }
}

// CSV�t�@�C�����t�@�C���o�͂���
template <typename T>
void CSVFile<T>::csv_write(string filepath, char delim) {
    // ��������csv�t�@�C�����J��(ofstream�̃R���X�g���N�^�ŊJ��)
    ofstream ofs_csv_file(filepath);

    if (isHeader) {
        for (int j = 0; j < header.size(); j++) {
            if (isIndex && j == 0) ofs_csv_file << delim;
            ofs_csv_file << header.at(j) << delim;
        }
        ofs_csv_file << endl;
    }
    for (int i = 0; i < index.size(); i++) {
        if (isIndex) {
            ofs_csv_file << index.at(i) << delim;
        }
        for (int j = 0; j < header.size(); j++) {
            ofs_csv_file << cell.at(i).at(j) << delim;
        }
        ofs_csv_file << endl;
    }
    ofs_csv_file << endl;
}

// CSV�t�@�C�����R���\�[���o�͂���
template <typename T>
void CSVFile<T>::csv_show() {
    cout << "filepath = " << filepath << ", ";
    cout << "isHeader = " << int(isHeader) << ", ";
    cout << "isIndex = " << int(isIndex) << ", ";
    cout << "delim = " << delim << endl;
    cout << "header size = " << header.size() << ", ";
    cout << "index size = " << index.size() << endl;

    if (isHeader) {
        for (int j = 0; j < header.size(); j++) {
            if (isIndex && j == 0) cout << "\t";
            cout << header.at(j) << "(h)" << "\t";
        }
        cout << endl;
    }
    for (int i = 0; i < index.size(); i++) {
        if (isIndex) {
            cout << index.at(i) << "(i)" << "\t";
        }
        for (int j = 0; j < header.size(); j++) {
            cout << cell.at(i).at(j) << "\t";
        }
        cout << endl;
    }
    cout << endl;
}
