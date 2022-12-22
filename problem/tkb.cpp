#include <iostream>
#include <cstdlib>
#include <ctime>

const int SoQuanThe = 100; // số lượng quần thể
const int SoGiaoVien = 10;     // số lượng giáo viên
const int SoPhongHoc = 5;    // số phòng học
const int SoLopHoc = 20;      // số lớp học
const int SoTietHoc = 10;    // số tiết học

// lịch biểu mã hoá thành mảng
int LichBieu[SoPhongHoc][SoTietHoc];

// độ thích nghi
double fitness[SoQuanThe];

// tạo 1 lịch biểu ngẫu nhiên 
void RandomLichBieu()
{
    for (int i = 0; i < SoPhongHoc; i++)
    {
        for (int j = 0; j < SoTietHoc; j++)
        {
            LichBieu[i][j] = rand() % SoLopHoc;
        }
    }
}

// tạo giáo viên dạy lớp học cho trước
int GiaoVienLopHoc(int course) // đối  số tạm thời chưa dùng
{
    // chỉ định ngẫu nhiên
    return rand() % SoGiaoVien;
}
// đánh giá độ phù hợp của lịch biểu
double DotPhuHop()
{
    double score = 0;

  // tính sự xung đột trong thời khoá biểu
    int XungDot = 0;
    for (int i = 0; i < SoPhongHoc; i++)
    {
        for (int j = 0; j < SoTietHoc; j++)
        {
            if (LichBieu[i][j] != -1)
            {
                for (int k = 0; k < SoPhongHoc; k++)
                {
                    if (k != i && LichBieu[k][j] == LichBieu[i][j])
                    {
                        XungDot++;
                    }
                }
            }
        }
    }

    // giáo viên bị phân công dạy 1 môn nhiều lúc
    for (int i = 0; i < SoGiaoVien; i++)
    {
        for (int j = 0; j < SoTietHoc; j++)
        {
            int mon1 = -1;
            int mon2 = -1;
            for (int k = 0; k < SoPhongHoc; k++)
            {
                if (LichBieu[k][j] != -1 && GiaoVienLopHoc(LichBieu[k][j]) == i)
                {
                    if (mon1 == -1)
                    {
                        mon1 = LichBieu[k][j];
                    }
                    else
                    {
                        mon2 = LichBieu[k][j];
                    }
                }
            }

            if (mon1 != -1 && mon2 != -1)
            {
                XungDot++;
            }
        }
    }

    // tính độ phù hợp
    score = 1.0 / (double)(XungDot + 1);

    return score;
}

void TinhLichBieu(int QuanThe[SoQuanThe][SoPhongHoc][SoTietHoc],
                       double fitness[SoQuanThe],
                       int numTheHes,
                       double mutationRate)
{
    const int So_TheHe = Toida; // thế hệ tối đa
    const double DotBien = XacXuat;  // xác xuất đột biến
    int TheHe = 1;
    double minFitness = 0.02;
    while (true)
    {
        //tạo quần áo mới cho thế hệ tiếp theo
        int newQuanThe[SoQuanThe][SoPhongHoc][SoTietHoc];
        double newFitness[SoQuanThe];

       // chọn bố mẹ cho thế hệ tiếp theo
        for (int i = 0; i < SoQuanThe; i++)
        {
            // chọn ngẫu nhiên 2 cá thể từ quần thể hiện tại
            int individual1 = rand() % SoQuanThe;
            int individual2 = rand() % SoQuanThe;

            // chọn cá thể cao hơn làm cha
            int parent = (fitness[individual1] > fitness[individual2]) ? individual1 : individual2;

           // tạo quần thể mới
            for (int j = 0; j < SoPhongHoc; j++)
            {
                for (int k = 0; k < SoTietHoc; k++)
                {
                    newQuanThe[i][j][k] = QuanThe[parent][j][k];
                }
            }
            newFitness[i] = fitness[parent];
        }

        // sử dụng phép lai đột biến
        for (int i = 0; i < SoQuanThe; i += 2)
        {
          // tạo lai chéo 
            int crossoverPoint = rand() % SoTietHoc;
            for (int j = 0; j < SoPhongHoc; j++)
            {
                for (int k = 0; k < crossoverPoint; k++)
                {
                    newQuanThe[i][j][k] = QuanThe[i][j][k];
                    newQuanThe[i + 1][j][k] = QuanThe[i + 1][j][k];
                }
                for (int k = crossoverPoint; k < SoTietHoc; k++)
                {
                    newQuanThe[i][j][k] = QuanThe[i + 1][j][k];
                    newQuanThe[i + 1][j][k] = QuanThe[i][j][k];
                }
            }

           // đột biến con với xác xuất là  tỉ lệ đột biến
            if (rand() < TiLeDB * RAND_MAX)
            {
               // lấy gen ngẫu nhiên
                int LopHoc = rand() % SoPhongHoc;
                int timeslot = rand() % SoTietHoc;

                newQuanThe[i][LopHoc][timeslot] = rand() % SoLopHoc;
            }

            if (rand() < TiLeDB * RAND_MAX)
            {
  
                int LopHoc = rand() % SoPhongHoc;
                int TGhoc = rand() % SoTietHoc;
                newQuanThe[i + 1][LopHoc][TGhoc] = rand() % SoLopHoc;
            } 
            newFitness[i] = DotPhuHop();
            newFitness[i + 1] = DotPhuHop();
        }

 		// Thay thế những thành viên yếu nhất của QuanThe hiện tại bằng gen mới
        for (int i = 0; i < SoQuanThe; i++)
        {
            int CTtot = i;
            for (int j = i + 1; j < SoQuanThe; j++)
            {
                if (fitness[j] < fitness[CTtot])
                {
                    CTtot = j;
                }
            }

          // kiểm tra gen con cso tot trong quen hệ hông
            if (newFitness[i] > fitness[CTtot])
            {
                for (int j = 0; j < SoPhongHoc; j++)
                {
                    for (int k = 0; k < SoTietHoc; k++)
                    {
                        QuanThe[CTtot][j][k] = newQuanThe[i][j][k];
                    }
                }
                fitness[CTtot] = newFitness[i];
            }
        } 
        TheHe++;

        if (TheHe >= So_TheHe)
        {
            break;
        }

     
        for (int i = 0; i < SoQuanThe; i++)
        {
            if (fitness[i] >= minFitness)
            {
        
                std::cout << "Giai phap " << TheHe << " The he:" << std::endl;
                std::cout << "LichBieu:" << std::endl;
                for (int j = 0; j < SoPhongHoc; j++)
                {
                    for (int k = 0; k < SoTietHoc; k++)
                    {
                        std::cout << QuanThe[i][j][k] << " ";
                    }
                    std::cout << std::endl;
                }
                std::cout << "Thich Hop: " << fitness[i] << std::endl;

                break;
            }
        }
    }
}

int main()
{
    srand(time(0));
   // khởi thạo nhiều lịch biểu ngẫu nhiên
    for (int i = 0; i < SoQuanThe; i++)
    {
        RandomLichBieu();
        fitness[i] = DotPhuHop();
    }

    // dùng thuật toán duy truyền để giải 
    int QuanThe[SoQuanThe][SoPhongHoc][SoTietHoc];
    for (int i = 0; i < SoQuanThe; i++)
    {
        for (int j = 0; j < SoPhongHoc; j++)
        {
            for (int k = 0; k < SoTietHoc; k++)
            {
                QuanThe[i][j][k] = LichBieu[j][k];
            }
        }
    }
    TinhLichBieu(QuanThe, fitness, 100, 0.01);

    return 0;
}