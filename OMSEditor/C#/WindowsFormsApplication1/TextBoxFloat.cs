using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WindowsFormsApplication1
{
    class TextBoxFloat : TextBox
    {
        protected override void OnKeyPress(KeyPressEventArgs e)
        {
            base.OnKeyPress(e);

            // . を許可
            if (e.KeyChar == '.')
            {
                // . が既にある
                if (this.Text.IndexOf('.') >= 0 ||
                    // 数値が入力されてない
                    this.Text.Length == 0)
                {
                    e.Handled = true;
                }
            }
            // - を許可
            else if (e.KeyChar == '-')
            {
                // - が既にある
                if (this.Text.IndexOf('-') >= 0 ||
                    // カーソル位置が最初でない
                    this.SelectionStart != 0)
                {
                    e.Handled = true;
                }
            }
            // back space 以外
            else if (e.KeyChar != '\b' &&
                // 0 ~ 9 以外
                (e.KeyChar < '0' || '9' < e.KeyChar))
                e.Handled = true;
        }
    }
}
