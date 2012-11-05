namespace SipicWindows
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.memoryView1 = new System.Windows.Forms.DataGridView();
            this.AddressColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ValueColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.button1 = new System.Windows.Forms.Button();
            this.Run = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.BPupdown = new System.Windows.Forms.NumericUpDown();
            this.numPC = new System.Windows.Forms.NumericUpDown();
            this.codeGridView = new System.Windows.Forms.DataGridView();
            this.numOPC = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.memoryView2 = new System.Windows.Forms.DataGridView();
            this.dataGridViewTextBoxColumn1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.dataGridViewTextBoxColumn2 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            ((System.ComponentModel.ISupportInitialize)(this.memoryView1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.BPupdown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numPC)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.codeGridView)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numOPC)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.memoryView2)).BeginInit();
            this.SuspendLayout();
            // 
            // memoryView1
            // 
            this.memoryView1.AllowUserToAddRows = false;
            this.memoryView1.AllowUserToDeleteRows = false;
            this.memoryView1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.memoryView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.memoryView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.AddressColumn,
            this.ValueColumn});
            this.memoryView1.EnableHeadersVisualStyles = false;
            this.memoryView1.Location = new System.Drawing.Point(12, 12);
            this.memoryView1.Name = "memoryView1";
            this.memoryView1.ReadOnly = true;
            this.memoryView1.RowHeadersVisible = false;
            this.memoryView1.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.memoryView1.ShowCellErrors = false;
            this.memoryView1.ShowCellToolTips = false;
            this.memoryView1.ShowEditingIcon = false;
            this.memoryView1.Size = new System.Drawing.Size(206, 496);
            this.memoryView1.TabIndex = 0;
            this.memoryView1.CellContentClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView1_CellContentClick);
            // 
            // AddressColumn
            // 
            this.AddressColumn.HeaderText = "Address";
            this.AddressColumn.Name = "AddressColumn";
            this.AddressColumn.ReadOnly = true;
            // 
            // ValueColumn
            // 
            this.ValueColumn.HeaderText = "Value";
            this.ValueColumn.Name = "ValueColumn";
            this.ValueColumn.ReadOnly = true;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(436, 12);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 1;
            this.button1.Text = "Step";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // Run
            // 
            this.Run.Location = new System.Drawing.Point(436, 41);
            this.Run.Name = "Run";
            this.Run.Size = new System.Drawing.Size(75, 23);
            this.Run.TabIndex = 2;
            this.Run.Text = "Run";
            this.Run.UseVisualStyleBackColor = true;
            this.Run.Click += new System.EventHandler(this.Run_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(553, 18);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(24, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "PC:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(553, 44);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(24, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "BP:";
            // 
            // BPupdown
            // 
            this.BPupdown.Hexadecimal = true;
            this.BPupdown.Increment = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.BPupdown.Location = new System.Drawing.Point(583, 44);
            this.BPupdown.Maximum = new decimal(new int[] {
            16777214,
            0,
            0,
            0});
            this.BPupdown.Name = "BPupdown";
            this.BPupdown.Size = new System.Drawing.Size(100, 20);
            this.BPupdown.TabIndex = 5;
            this.BPupdown.Value = new decimal(new int[] {
            2652,
            0,
            0,
            0});
            // 
            // numPC
            // 
            this.numPC.Enabled = false;
            this.numPC.Hexadecimal = true;
            this.numPC.Increment = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.numPC.Location = new System.Drawing.Point(583, 16);
            this.numPC.Maximum = new decimal(new int[] {
            16777214,
            0,
            0,
            0});
            this.numPC.Name = "numPC";
            this.numPC.ReadOnly = true;
            this.numPC.Size = new System.Drawing.Size(100, 20);
            this.numPC.TabIndex = 6;
            // 
            // codeGridView
            // 
            this.codeGridView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.codeGridView.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.codeGridView.Location = new System.Drawing.Point(436, 71);
            this.codeGridView.Name = "codeGridView";
            this.codeGridView.Size = new System.Drawing.Size(685, 437);
            this.codeGridView.TabIndex = 7;
            // 
            // numOPC
            // 
            this.numOPC.Enabled = false;
            this.numOPC.Hexadecimal = true;
            this.numOPC.Increment = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.numOPC.Location = new System.Drawing.Point(727, 16);
            this.numOPC.Maximum = new decimal(new int[] {
            16777214,
            0,
            0,
            0});
            this.numOPC.Name = "numOPC";
            this.numOPC.ReadOnly = true;
            this.numOPC.Size = new System.Drawing.Size(100, 20);
            this.numOPC.TabIndex = 6;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(689, 18);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(32, 13);
            this.label3.TabIndex = 4;
            this.label3.Text = "OPC:";
            // 
            // memoryView2
            // 
            this.memoryView2.AllowUserToAddRows = false;
            this.memoryView2.AllowUserToDeleteRows = false;
            this.memoryView2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.memoryView2.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.memoryView2.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.dataGridViewTextBoxColumn1,
            this.dataGridViewTextBoxColumn2});
            this.memoryView2.EnableHeadersVisualStyles = false;
            this.memoryView2.Location = new System.Drawing.Point(224, 12);
            this.memoryView2.Name = "memoryView2";
            this.memoryView2.ReadOnly = true;
            this.memoryView2.RowHeadersVisible = false;
            this.memoryView2.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.memoryView2.ShowCellErrors = false;
            this.memoryView2.ShowCellToolTips = false;
            this.memoryView2.ShowEditingIcon = false;
            this.memoryView2.Size = new System.Drawing.Size(206, 496);
            this.memoryView2.TabIndex = 0;
            this.memoryView2.CellContentClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView1_CellContentClick);
            // 
            // dataGridViewTextBoxColumn1
            // 
            this.dataGridViewTextBoxColumn1.HeaderText = "Address";
            this.dataGridViewTextBoxColumn1.Name = "dataGridViewTextBoxColumn1";
            this.dataGridViewTextBoxColumn1.ReadOnly = true;
            // 
            // dataGridViewTextBoxColumn2
            // 
            this.dataGridViewTextBoxColumn2.HeaderText = "Value";
            this.dataGridViewTextBoxColumn2.Name = "dataGridViewTextBoxColumn2";
            this.dataGridViewTextBoxColumn2.ReadOnly = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1133, 520);
            this.Controls.Add(this.codeGridView);
            this.Controls.Add(this.numOPC);
            this.Controls.Add(this.numPC);
            this.Controls.Add(this.BPupdown);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.Run);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.memoryView2);
            this.Controls.Add(this.memoryView1);
            this.Name = "Form1";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.memoryView1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.BPupdown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numPC)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.codeGridView)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numOPC)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.memoryView2)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.DataGridView memoryView1;
        private System.Windows.Forms.DataGridViewTextBoxColumn AddressColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn ValueColumn;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button Run;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.NumericUpDown BPupdown;
        private System.Windows.Forms.NumericUpDown numPC;
        private System.Windows.Forms.DataGridView codeGridView;
        private System.Windows.Forms.NumericUpDown numOPC;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.DataGridView memoryView2;
        private System.Windows.Forms.DataGridViewTextBoxColumn dataGridViewTextBoxColumn1;
        private System.Windows.Forms.DataGridViewTextBoxColumn dataGridViewTextBoxColumn2;
    }
}

