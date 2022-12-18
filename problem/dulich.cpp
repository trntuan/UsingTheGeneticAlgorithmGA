// C++ implementation of the above approach
#include <bits/stdc++.h> // dùng cho các vector để tính nhanh việc sort...
#include <limits.h>
using namespace std;

// số thành phố
#define V 5


// kích thước quần thể ban đầu cho thuật toán
#define Kich_Thuoc_QT 10

// cấu trúc của 1 nhiễm sắc thể 
// Ca_The định nghĩa đường đi các thành phố qua bởi người bán hàng
// Gia_Tri là giá trị của đường đi qua đó 
struct Mo_Ta {
	string Ca_The;
	int Gia_Tri;
};

// hàm trả về giá trị random từ end và start
int rand_num(int start, int end)
{
	int r = end - start; 	// VD end = 10, start = 1 r = 9
	int rnum = start + rand() % r;  // rand 0 - 32767 | vd 8 % 9 = 8 | 8 + 1 = 9   
	return rnum; // 9
}


// hàm kiểm tra 1 chữ có nằm trong chuỗi hay không 
bool repeat(string s, char ch)
{
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == ch) // nếu có tồn tại
			return true;
	}
	return false;
}

// hàm thay đổi vị trí 2 phần tử trong 1 gen để tạo ra gen mới ( đổi 2 kí tự của 1 string )
string DotBien(string Ca_The)
{
	while (true) {
		int r = rand_num(1, V); // trị ramdom trong từ 1-10 thứ 1 
		int r1 = rand_num(1, V); // trị ramdom trong từ 1-10 thứ 2
		// nếu 2 giá trị ramdom khác nhau thì đổi chổ 2 vị ngẫu nhiên trong chuỗi gen,tạo đột biến
		if (r1 != r) {  
			char temp = Ca_The[r];
			Ca_The[r] = Ca_The[r1];
			Ca_The[r1] = temp;
			break;
		}
	}
	return Ca_The;
}

// hàm tạo một chuỗi gen đúng 
string create_Ca_The()
{
	string Ca_The = "0"; // bắt đầu từ thành phố 0 
	while (true) {
		if (Ca_The.size() == V) { // nếu độ dài gen đã đủ thì về thành phố 0
			Ca_The += Ca_The[0];
			break;
		}
		int temp = rand_num(1, V); // tạo một thành phố ngẫu nhiên
		if (!repeat(Ca_The, (char)(temp + 48))) // nếu thành phố đó chưa tồn tại trong gen thì thêm vào
			Ca_The += (char)(temp + 48); // chuyển đổi số int thành kiểu char ở mã ASCII để gắn vào gen
	}
	return Ca_The;//trả về 1 chuỗi gen
}
// lai 2 cá thể với nhau
//string Lai_Tao(string ct_A, string ct_B)
//{
//	string con = "0";
//	int nstCon = 1;
//	int tam =0;
//	for(int nstCon;nstCon<3;nstCon++)// chạy đến 1 nữa các thể A
//	{
//		con += ct_A[nstCon];
//	}
//	while(nstCon == ct_A.size())
//	{
//		(!repeat(Ca_The, ct_B[tam])
//			{
//	 		 			 
//			con += ct_B[nstCon];		
//				nstCon++;			  			   		  	
// 			}
//			tam ++;				 
//	}
//	con += '0'; // trở về thành phố 0;
//	return con;
//	
//}

// trả về giá trị gen phù hợp ( không phải INT_MAX)
int cal_Gia_Tri(string Ca_The)
{ // tạo mảng 2 chiều
	int map[V][V] = { { 0, 2, INT_MAX, 12, 5 }, // INT_MAX = 2147483647  <=> + vô cùng
					{ 2, 0, 4, 8, INT_MAX },
					{ INT_MAX, 4, 0, 3, 3 },
					{ 12, 8, 3, 0, 10 },
					{ 5, INT_MAX, 3, 10, 0 } };
	int f = 0;
	for (int i = 0; i < Ca_The.size() - 1; i++) {
		// nếu tồn tại đường đi ngược chiều thì gen đó không phù hợp
		if (map[Ca_The[i] - 48][Ca_The[i + 1] - 48] == INT_MAX) 
			return INT_MAX;
		// nếu gen phù hợp thì tính tổng độ dài của đường đi chuỗi gen đó 
		f += map[Ca_The[i] - 48][Ca_The[i + 1] - 48];
	}
	return f;
}

// so sánh giá trị của 2 bộ gen
bool Giam_Dan(struct Mo_Ta t1,
			struct Mo_Ta t2)
{
	return t1.Gia_Tri < t2.Gia_Tri;
}

//hàm thực hiện bài toán TSP
void TSPUtil(int map[V][V])
{
	// biến để tính số thế hệ hiện tại
	int TheHe = 1; 
	// số lần lặp ( số thế hệ )
	int SoTheHe = 5;

	vector<struct Mo_Ta> Quan_The; // tạo quần thể kiểu gen
	struct Mo_Ta temp; // tạo 1 biến gen tạm

	// tạo 1 quần thể gồm mỗi cá thể có một đoạn gen
	for (int i = 0; i < Kich_Thuoc_QT; i++) {
		temp.Ca_The = create_Ca_The(); // tạo gen
		temp.Gia_Tri = cal_Gia_Tri(temp.Ca_The); // giá trị của đường đi của gen
		Quan_The.push_back(temp); // thêm phần tử đó vào vector Quan_The
	}
	
	// in ra quần thể hiện tại
	cout << "\n Quan_The dau: " << endl 
		<< "Ca_The	 Gia_Tri VALUE\n";
	for (int i = 0; i < Kich_Thuoc_QT; i++) // in ra từng gen
		cout << Quan_The[i].Ca_The << " "
			<< Quan_The[i].Gia_Tri << endl;
	cout << "\n";


	// lập lại việc lai quần thể và đột biến
	while (TheHe <= SoTheHe) { // nếu nhiệt độ và thế hệ chưa đạt thì
		sort(Quan_The.begin(), Quan_The.end(), Giam_Dan); // xắp xếp 
		vector<struct Mo_Ta> new_Quan_The; // tạo quần thể mới

		for (int i = 0; i < Kich_Thuoc_QT; i++) {
			struct Mo_Ta p1 = Quan_The[i]; // 1 tạm để dùng từng các gen trong quần thể

			while (true) {
				string new_g = DotBien(p1.Ca_The); //đột biến gen mới dựa vào gen ở trên
				struct Mo_Ta new_Ca_The; // tạo ra một mô tả của gen mới
				new_Ca_The.Ca_The = new_g; // gán đường đi của gen mới bằng giá trị vừa đột biến
				new_Ca_The.Gia_Tri = cal_Gia_Tri(new_Ca_The.Ca_The); // lấy độ dài của gen mới 
		  		
		  		// nếu độ dài của gen vừa đột biến tốt hơn gen cũ thì thêm nó vào quần thể mới
				if (new_Ca_The.Gia_Tri <= Quan_The[i].Gia_Tri) { 
					new_Quan_The.push_back(new_Ca_The);
					break;
				}
				else {
				new_Quan_The.push_back(Quan_The[i]);
				break;  	  
				}
			}
		}

		Quan_The = new_Quan_The;
		cout << "The He " << TheHe << " \n";
		cout << "Quan The  \n";

		for (int i = 0; i < Kich_Thuoc_QT; i++)
			cout << Quan_The[i].Ca_The << " "
				<< Quan_The[i].Gia_Tri << endl;
		TheHe++;
	}
}

int main()
{

	int map[V][V] = { { 0, 2, INT_MAX, 12, 5 },
					{ 2, 0, 4, 8, INT_MAX },
					{ INT_MAX, 4, 0, 3, 3 },
					{ 12, 8, 3, 0, 10 },
					{ 5, INT_MAX, 3, 10, 0 } };
	TSPUtil(map);
}
