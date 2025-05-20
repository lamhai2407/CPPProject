# CPPProject
# Dự án ví điểm thưởng

Thành viên tạo dự án: Phan Lâm Hải - K24DTCN053

Mô tả chung về dự án:
  *Người dùng (User) có thể tạo tài khoản ví điểm và thực hiện các thao tác mua bán trao đổi điểm giữa các ví
  
  *Quản lý (Admin) sẽ có các chức năng như hỗ trợ thay đổi thông tin người dùng, nạp thêm/trừ bớt điểm ở ví người dùng, quản lý thông tin nhóm người dùng
  
  *Các thao tác liên quan sẽ phải được xác thực với mã OTP

Mô tả chi tiết các chức năng trong chương trình:

Chức năng chính
1. Quản lý người dùng
-Đăng ký người dùng mới, có tùy chọn tạo mật khẩu tạm.

-Đăng nhập bằng tên người dùng và mật khẩu đã mã hóa.

-Đổi tên đăng nhập hoặc mật khẩu (có xác thực OTP).

-Kiểm tra số dư ví điểm.

-Chuyển điểm đến tài khoản khác (có xác thực OTP).

-Xem lịch sử giao dịch của bản thân.

2. Quản lý quản trị viên (admin)
-Tạo tài khoản admin với OTP xác nhận.

-Đăng nhập admin bằng tài khoản và mật khẩu.

-Xem danh sách người dùng và số dư của họ.

-Điều chỉnh điểm trong ví người dùng (tăng/giảm) có xác nhận OTP.

-Đặt lại thông tin tài khoản người dùng (tên/mật khẩu).

3. Bảo mật
-Mật khẩu được mã hóa bằng hàm băm đơn giản (simpleHash()).

-Mỗi hành động quan trọng đều cần xác minh bằng OTP sinh ngẫu nhiên.

-OTP và mật khẩu tạm giúp tăng tính bảo mật cho những thao tác quan trọng.

Cấu trúc dữ liệu và lưu trữ
-Dữ liệu người dùng được lưu trong:

users.txt (danh sách tổng hợp)

-<username>.txt (file riêng của từng người dùng, chứa lịch sử giao dịch)

-Dữ liệu admin lưu trong admins.txt.

-Sử dụng unordered_map để lưu người dùng và admin trong bộ nhớ trong thời gian chạy
  
  
